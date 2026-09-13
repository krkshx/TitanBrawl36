#pragma once
// TSP-сокет: TCP-транспорт Titan Socket Protocol.
// Кадр: type u16be | len u24be | version u16be, затем payload.
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define TSPLOG(fmt, ...) std::fprintf(stderr, "[tsp] " fmt "\n", ##__VA_ARGS__)

class TspSocket {
public:
    static constexpr int kHeaderSize = 7;
    static constexpr int kMaxPayload = 4 * 1024 * 1024;

    TspSocket() = default;
    ~TspSocket() { close(); }

    TspSocket(const TspSocket &) = delete;
    TspSocket &operator=(const TspSocket &) = delete;

    TspSocket(TspSocket &&other) noexcept : fd_(other.fd_), error_(std::move(other.error_)), peer_(std::move(other.peer_)) {
        other.fd_ = -1;
    }
    TspSocket &operator=(TspSocket &&other) noexcept {
        if (this != &other) {
            close();
            fd_ = other.fd_;
            error_ = std::move(other.error_);
            peer_ = std::move(other.peer_);
            other.fd_ = -1;
        }
        return *this;
    }

    void close() {
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    bool isOpen() const { return fd_ >= 0; }
    int fd() const { return fd_; }
    // Path MTU коннекта (орига кладёт его в LoginMessage.RndKey).
    int pathMtu(int fallback = 1400) const {
        if (fd_ < 0) {
            return fallback;
        }
        int mtu = 0;
        socklen_t len = sizeof(mtu);
        if (getsockopt(fd_, IPPROTO_IP, IP_MTU, &mtu, &len) != 0 || mtu <= 0) {
            return fallback;
        }
        return mtu;
    }
    const std::string &lastError() const { return error_; }
    const std::string &peer() const { return peer_; }

    bool connect(const char *host, std::int32_t port, int timeoutMs = 5000) {
        close();
        error_.clear();
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0) {
            error_ = "socket failed";
            return false;
        }
        int flags = fcntl(fd_, F_GETFL, 0);
        fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(static_cast<std::uint16_t>(port));
        if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
            error_ = "bad host";
            close();
            return false;
        }
        int rc = ::connect(fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
        if (rc < 0 && errno != EINPROGRESS) {
            error_ = std::string("connect: ") + std::strerror(errno);
            close();
            return false;
        }
        if (!waitWritable(timeoutMs)) {
            error_ = "connect timeout";
            close();
            return false;
        }
        fcntl(fd_, F_SETFL, flags);
        int soErr = 0;
        socklen_t soLen = sizeof(soErr);
        getsockopt(fd_, SOL_SOCKET, SO_ERROR, &soErr, &soLen);
        if (soErr != 0) {
            error_ = std::string("connect: ") + std::strerror(soErr);
            close();
            return false;
        }
        int one = 1;
        setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
        setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(one));
        char ip[64];
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
        peer_ = std::string(ip) + ":" + std::to_string(port);
        TSPLOG("connect %s OK fd=%d", peer_.c_str(), fd_);
        return true;
    }

    bool sendFrame(std::int32_t type, std::int32_t version, const char *payload, std::int32_t len) {
        if (!isOpen()) {
            error_ = "not connected";
            return false;
        }
        if (len < 0 || len > kMaxPayload) {
            error_ = "bad frame len";
            return false;
        }
        std::uint8_t hdr[kHeaderSize];
        hdr[0] = static_cast<std::uint8_t>((type >> 8) & 0xFF);
        hdr[1] = static_cast<std::uint8_t>(type & 0xFF);
        hdr[2] = static_cast<std::uint8_t>((len >> 16) & 0xFF);
        hdr[3] = static_cast<std::uint8_t>((len >> 8) & 0xFF);
        hdr[4] = static_cast<std::uint8_t>(len & 0xFF);
        hdr[5] = static_cast<std::uint8_t>((version >> 8) & 0xFF);
        hdr[6] = static_cast<std::uint8_t>(version & 0xFF);
        if (!sendAll(hdr, kHeaderSize)) {
            return false;
        }
        if (len > 0 && !sendAll(reinterpret_cast<const std::uint8_t *>(payload), len)) {
            return false;
        }
        return true;
    }

    bool recvFrame(std::int32_t &type, std::int32_t &version, std::vector<std::uint8_t> &payload, int timeoutMs) {
        if (!isOpen()) {
            error_ = "not connected";
            return false;
        }
        std::uint8_t hdr[kHeaderSize];
        if (!recvAll(hdr, kHeaderSize, timeoutMs)) {
            return false;
        }
        type = (hdr[0] << 8) | hdr[1];
        std::int32_t len = (hdr[2] << 16) | (hdr[3] << 8) | hdr[4];
        version = (hdr[5] << 8) | hdr[6];
        if (len < 0 || len > kMaxPayload) {
            error_ = "bad frame len";
            return false;
        }
        payload.resize(static_cast<std::size_t>(len));
        if (len > 0 && !recvAll(payload.data(), len, timeoutMs)) {
            return false;
        }
        return true;
    }

private:
    bool waitWritable(int timeoutMs) {
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(fd_, &wfds);
        timeval tv{};
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;
        int rc = select(fd_ + 1, nullptr, &wfds, nullptr, &tv);
        return rc > 0;
    }

    bool sendAll(const std::uint8_t *buf, int n) {
        int sent = 0;
        while (sent < n) {
            ssize_t r = ::send(fd_, buf + sent, static_cast<std::size_t>(n - sent), MSG_NOSIGNAL);
            if (r <= 0) {
                error_ = r == 0 ? "send closed" : std::string("send: ") + std::strerror(errno);
                return false;
            }
            sent += static_cast<int>(r);
        }
        return true;
    }

    bool recvAll(std::uint8_t *buf, int n, int timeoutMs) {
        int got = 0;
        while (got < n) {
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(fd_, &rfds);
            timeval tv{};
            tv.tv_sec = timeoutMs / 1000;
            tv.tv_usec = (timeoutMs % 1000) * 1000;
            int rc = select(fd_ + 1, &rfds, nullptr, nullptr, &tv);
            if (rc <= 0) {
                error_ = rc == 0 ? "recv timeout" : "select failed";
                return false;
            }
            ssize_t r = ::recv(fd_, buf + got, static_cast<std::size_t>(n - got), 0);
            if (r == 0) {
                error_ = "connection closed";
                return false;
            }
            if (r < 0) {
                if (errno == EINTR) {
                    continue;
                }
                error_ = std::string("recv: ") + std::strerror(errno);
                return false;
            }
            got += static_cast<int>(r);
        }
        return true;
    }

    int fd_ = -1;
    std::string error_;
    std::string peer_;
};
