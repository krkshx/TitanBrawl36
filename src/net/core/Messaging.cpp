#pragma once
#include "PiranhaMessage.cpp"
#include "MessageFactory.cpp"
#include "ServerConfig.cpp"
#include "TspSocket.cpp"
#include "../../titan/crypto/PepperCrypto.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NETLOG(fmt, ...) std::fprintf(stderr, "[net] " fmt "\n", ##__VA_ARGS__)

class ClientMessaging {
public:
    ClientMessaging() = default;
    ~ClientMessaging() { close(); }

    void close() { socket_.close(); }
    bool isOpen() const { return socket_.isOpen(); }
    int pathMtu() const { return socket_.pathMtu(); }
    const std::string &lastError() const { return error_.empty() ? socket_.lastError() : error_; }

    bool connect(int timeoutMs = 5000) {
        close();
        error_.clear();
        if (!PepperCrypto::init()) {
            error_ = "sodium_init failed";
            return false;
        }
        // TSP-коннект к серверу из ServerConfig (93.123.84.82:9339).
        return socket_.connect(ServerConfig::host(), ServerConfig::port(), timeoutMs);
    }

    bool sendClientHello() {
        ClientHelloMessage hello;
        hello.encode();
        NETLOG("send 10100 hello proto=2 keyVer=24 v=36.0.218 len=%d", hello.getByteStream()->getLength());
        return socket_.sendFrame(hello.getMessageType(), hello.getMessageVersion(), hello.getByteStream()->getByteArray(), hello.getByteStream()->getLength());
    }

    bool receiveServerHello(std::vector<std::uint8_t> &token, int timeoutMs = 8000) {
        std::int32_t type = 0;
        std::vector<std::uint8_t> payload;
        std::int32_t frameVersion = 0;
        if (!socket_.recvFrame(type, frameVersion, payload, timeoutMs)) {
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
        NETLOG("recv 20100 serverHello tokenLen=%zu", token.size());
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
        NETLOG("send 10101 pepperLogin bodyLen=%d packetLen=%zu", bodyLen, packet.size());
        // Версия фрейма = версия сообщения (у LoginMessage это 10 из TitanLoginMessage).
        return socket_.sendFrame(10101, login.getMessageVersion(), reinterpret_cast<const char *>(packet.data()), static_cast<std::int32_t>(packet.size()));
    }

    PiranhaMessage *receivePepperResponse(bool expectCreate, int timeoutMs = 8000) {
        std::int32_t type = 0;
        std::vector<std::uint8_t> payload;
        std::int32_t frameVersion = 0;
        if (!socket_.recvFrame(type, frameVersion, payload, timeoutMs)) {
            NETLOG("pepperResponse recv FAIL %s", error_.c_str());
            return nullptr;
        }
        NETLOG("recv frame type=%d len=%zu", type, payload.size());
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
        std::int32_t frameVersion = 0;
        if (!socket_.recvFrame(type, frameVersion, payload, timeoutMs)) {
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
            return socket_.sendFrame(msg.getMessageType(), msg.getMessageVersion(), reinterpret_cast<const char *>(box.data()), static_cast<std::int32_t>(box.size()));
        }
        return socket_.sendFrame(msg.getMessageType(), msg.getMessageVersion(), reinterpret_cast<const char *>(plain.data()), static_cast<std::int32_t>(plain.size()));
    }

    bool streamOn() const { return streamOn_; }
    std::int64_t accountId() const { return accountId_; }
    const std::string &passToken() const { return passToken_; }

private:
    TspSocket socket_;
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

