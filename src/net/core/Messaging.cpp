#pragma once
#include "PiranhaMessage.cpp"
#include "MessageFactory.cpp"
#include "ServerConfig.cpp"
#include "../../titan/crypto/PepperCrypto.cpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class ClientMessaging {
public:
    ClientMessaging() = default;
    ~ClientMessaging() { close(); }

    void close() {
        if (sock_ >= 0) {
            ::close(sock_);
            sock_ = -1;
        }
    }

    bool connect(int timeoutMs = 5000) {
        close();
        if (!PepperCrypto::init()) {
            error_ = "sodium_init failed";
            return false;
        }
        sock_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock_ < 0) {
            error_ = "socket failed";
            return false;
        }
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(static_cast<std::uint16_t>(ServerConfig::port()));
        if (inet_pton(AF_INET, ServerConfig::host(), &addr.sin_addr) != 1) {
            error_ = "bad host";
            return false;
        }
        if (!waitWritable(timeoutMs)) {
            error_ = "connect timeout";
            return false;
        }
        int rc = ::connect(sock_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
        if (rc < 0 && errno != EINPROGRESS) {
            error_ = std::string("connect: ") + std::strerror(errno);
            return false;
        }
        if (!waitWritable(timeoutMs)) {
            error_ = "connect timeout";
            return false;
        }
        int soErr = 0;
        socklen_t soLen = sizeof(soErr);
        getsockopt(sock_, SOL_SOCKET, SO_ERROR, &soErr, &soLen);
        if (soErr != 0) {
            error_ = std::string("connect: ") + std::strerror(soErr);
            return false;
        }
        return true;
    }

    bool sendClientHello(std::int32_t seed) {
        ClientHelloMessage hello;
        hello.setClientSeed(seed);
        hello.encode();
        return sendFrame(hello.getMessageType(), hello.getMessageVersion(), hello.getByteStream()->getByteArray(), hello.getByteStream()->getLength());
    }

    bool receiveServerHello(std::vector<std::uint8_t> &token, int timeoutMs = 8000) {
        std::int32_t type = 0;
        std::vector<std::uint8_t> payload;
        if (!recvFrame(type, payload, timeoutMs)) {
            return false;
        }
        if (type != 20100) {
            error_ = "expected 20100, got " + std::to_string(type);
            return false;
        }
        ServerHelloMessage hello;
        hello.getByteStream()->setByteArray(reinterpret_cast<const char *>(payload.data()), static_cast<std::int32_t>(payload.size()));
        hello.decode();
        token = hello.token();
        if (token.size() != 24) {
            error_ = "bad session token len " + std::to_string(token.size());
            return false;
        }
        return true;
    }

    bool sendPepperLogin(LoginMessage &login, const std::vector<std::uint8_t> &sessionToken) {
        login.getByteStream()->clear(256);
        login.encode();
        const char *body = login.getByteStream()->getByteArray();
        std::int32_t bodyLen = login.getByteStream()->getLength();

        std::uint8_t sk[32];
        PepperCrypto::clientSecret(sk);
        if (!PepperCrypto::clientPublic(clientPk_)) {
            error_ = "scalarmult_base failed";
            return false;
        }
        std::memcpy(serverKey_, PEPPER_SERVER_KEY, 32);
        randomBytesTrue(rNonce_, 24);

        std::vector<std::uint8_t> plain;
        plain.reserve(48 + static_cast<std::size_t>(bodyLen));
        plain.insert(plain.end(), sessionToken.begin(), sessionToken.end());
        plain.insert(plain.end(), rNonce_, rNonce_ + 24);
        plain.insert(plain.end(), body, body + bodyLen);

        std::uint8_t nonce[24];
        if (!PepperCrypto::blake2b24(nonce, clientPk_, 32, serverKey_, 32)) {
            error_ = "nonce blake failed";
            return false;
        }
        std::vector<std::uint8_t> box(plain.size() + 16);
        if (!PepperCrypto::boxSeal(plain.data(), static_cast<int>(plain.size()), box.data(), nonce, serverKey_, sk)) {
            error_ = "box seal failed";
            return false;
        }
        std::vector<std::uint8_t> packet;
        packet.reserve(32 + box.size());
        packet.insert(packet.end(), clientPk_, clientPk_ + 32);
        packet.insert(packet.end(), box.begin(), box.end());
        sentAccountId_ = 0;
        return sendFrame(10101, 0, reinterpret_cast<const char *>(packet.data()), static_cast<std::int32_t>(packet.size()));
    }

    PiranhaMessage *receivePepperResponse(bool expectCreate, int timeoutMs = 8000) {
        std::int32_t type = 0;
        std::vector<std::uint8_t> payload;
        if (!recvFrame(type, payload, timeoutMs)) {
            return nullptr;
        }
        std::uint8_t nonce[24];
        std::uint8_t sk[32];
        PepperCrypto::clientSecret(sk);
        std::vector<std::uint8_t> open;
        if (PepperCrypto::blake2b24(nonce, rNonce_, 24, clientPk_, 32, serverKey_, 32) &&
            payload.size() > 56 + 16 &&
            (open.resize(payload.size() - 16), PepperCrypto::boxOpen(payload.data(), static_cast<int>(payload.size()), open.data(), nonce, serverKey_, sk))) {
            std::memcpy(sNonce_, open.data(), 24);
            std::uint8_t streamKey[32];
            std::memcpy(streamKey, open.data() + 24, 32);
            tx_.setup(streamKey, sNonce_);
            rx_.setup(streamKey, rNonce_);
            streamOn_ = true;
            std::vector<std::uint8_t> inner(open.begin() + 56, open.end());
            PiranhaMessage *msg = expectCreate ? static_cast<PiranhaMessage *>(new CreateAccountOkMessage()) : static_cast<PiranhaMessage *>(new LoginOkMessage());
            msg->getByteStream()->setByteArray(reinterpret_cast<const char *>(inner.data()), static_cast<std::int32_t>(inner.size()));
            msg->decode();
            if (expectCreate) {
                auto *c = static_cast<CreateAccountOkMessage *>(msg);
                accountId_ = c->accountId();
                passToken_ = c->passToken();
            } else {
                auto *o = static_cast<LoginOkMessage *>(msg);
                accountId_ = o->accountId();
                passToken_ = o->passToken();
            }
            return msg;
        }
        PiranhaMessage *msg = MessageFactory::create(type);
        if (msg == nullptr) {
            error_ = "unknown raw type " + std::to_string(type);
            return nullptr;
        }
        msg->getByteStream()->setByteArray(reinterpret_cast<const char *>(payload.data()), static_cast<std::int32_t>(payload.size()));
        msg->decode();
        return msg;
    }

    PiranhaMessage *receiveNext(int timeoutMs = 5000) {
        std::int32_t type = 0;
        std::vector<std::uint8_t> payload;
        if (!recvFrame(type, payload, timeoutMs)) {
            return nullptr;
        }
        if (streamOn_) {
            std::vector<std::uint8_t> plain;
            if (!rx_.decrypt(payload.data(), static_cast<int>(payload.size()), plain)) {
                error_ = "stream decrypt failed";
                return nullptr;
            }
            payload.swap(plain);
        }
        PiranhaMessage *msg = MessageFactory::create(type);
        if (msg == nullptr) {
            error_ = "unknown type " + std::to_string(type);
            return nullptr;
        }
        msg->getByteStream()->setByteArray(reinterpret_cast<const char *>(payload.data()), static_cast<std::int32_t>(payload.size()));
        msg->decode();
        if (msg->getMessageType() == 20104) {
            auto *o = static_cast<LoginOkMessage *>(msg);
            accountId_ = o->accountId();
            passToken_ = o->passToken();
        }
        return msg;
    }

    bool sendGame(PiranhaMessage &msg) {
        msg.getByteStream()->clear(128);
        msg.encode();
        std::vector<std::uint8_t> plain(msg.getByteStream()->getByteArray(), msg.getByteStream()->getByteArray() + msg.getByteStream()->getLength());
        if (streamOn_) {
            std::vector<std::uint8_t> box;
            if (!tx_.encrypt(plain, box)) {
                error_ = "stream encrypt failed";
                return false;
            }
            return sendFrame(msg.getMessageType(), msg.getMessageVersion(), reinterpret_cast<const char *>(box.data()), static_cast<std::int32_t>(box.size()));
        }
        return sendFrame(msg.getMessageType(), msg.getMessageVersion(), reinterpret_cast<const char *>(plain.data()), static_cast<std::int32_t>(plain.size()));
    }

    bool streamOn() const { return streamOn_; }
    std::int64_t accountId() const { return accountId_; }
    const std::string &passToken() const { return passToken_; }
    const std::string &lastError() const { return error_; }

private:
    bool waitWritable(int timeoutMs) {
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sock_, &wfds);
        timeval tv{};
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;
        int flags = fcntl(sock_, F_GETFL, 0);
        fcntl(sock_, F_SETFL, flags | O_NONBLOCK);
        int rc = select(sock_ + 1, nullptr, &wfds, nullptr, &tv);
        fcntl(sock_, F_SETFL, flags);
        return rc > 0;
    }

    bool recvAll(std::uint8_t *buf, int n, int timeoutMs) {
        int got = 0;
        while (got < n) {
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(sock_, &rfds);
            timeval tv{};
            tv.tv_sec = timeoutMs / 1000;
            tv.tv_usec = (timeoutMs % 1000) * 1000;
            int rc = select(sock_ + 1, &rfds, nullptr, nullptr, &tv);
            if (rc <= 0) {
                error_ = rc == 0 ? "recv timeout" : "select failed";
                return false;
            }
            ssize_t r = ::recv(sock_, buf + got, static_cast<std::size_t>(n - got), 0);
            if (r <= 0) {
                error_ = "connection closed";
                return false;
            }
            got += static_cast<int>(r);
        }
        return true;
    }

    bool sendAll(const std::uint8_t *buf, int n) {
        int sent = 0;
        while (sent < n) {
            ssize_t r = ::send(sock_, buf + sent, static_cast<std::size_t>(n - sent), 0);
            if (r <= 0) {
                error_ = "send failed";
                return false;
            }
            sent += static_cast<int>(r);
        }
        return true;
    }

    bool sendFrame(std::int32_t type, std::int32_t version, const char *payload, std::int32_t len) {
        std::uint8_t hdr[7];
        hdr[0] = static_cast<std::uint8_t>((type >> 8) & 0xFF);
        hdr[1] = static_cast<std::uint8_t>(type & 0xFF);
        hdr[2] = static_cast<std::uint8_t>((len >> 16) & 0xFF);
        hdr[3] = static_cast<std::uint8_t>((len >> 8) & 0xFF);
        hdr[4] = static_cast<std::uint8_t>(len & 0xFF);
        hdr[5] = static_cast<std::uint8_t>((version >> 8) & 0xFF);
        hdr[6] = static_cast<std::uint8_t>(version & 0xFF);
        if (!sendAll(hdr, 7)) {
            return false;
        }
        if (len > 0 && !sendAll(reinterpret_cast<const std::uint8_t *>(payload), len)) {
            return false;
        }
        return true;
    }

    bool recvFrame(std::int32_t &type, std::vector<std::uint8_t> &payload, int timeoutMs) {
        std::uint8_t hdr[7];
        if (!recvAll(hdr, 7, timeoutMs)) {
            return false;
        }
        type = (hdr[0] << 8) | hdr[1];
        std::int32_t len = (hdr[2] << 16) | (hdr[3] << 8) | hdr[4];
        if (len < 0 || len > 4 * 1024 * 1024) {
            error_ = "bad frame len";
            return false;
        }
        payload.resize(static_cast<std::size_t>(len));
        if (len > 0 && !recvAll(payload.data(), len, timeoutMs)) {
            return false;
        }
        return true;
    }

    int sock_ = -1;
    std::string error_;
    bool streamOn_ = false;
    std::uint8_t clientPk_[32] = {0};
    std::uint8_t serverKey_[32] = {0};
    std::uint8_t rNonce_[24] = {0};
    std::uint8_t sNonce_[24] = {0};
    PepperStream tx_;
    PepperStream rx_;
    std::int64_t sentAccountId_ = 0;
    std::int64_t accountId_ = 0;
    std::string passToken_;
};

