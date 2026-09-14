#pragma once
// TSP-сокет: TCP-транспорт Titan Socket Protocol.
// Кадр: type u16be | len u24be | version u16be, затем payload.
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#endif
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define TSPLOG(fmt, ...) std::fprintf(stderr, "[tsp] " fmt "\n", ##__VA_ARGS__)

#if defined(_WIN32)
using TspHandle = SOCKET;
constexpr TspHandle kTspInvalid = INVALID_SOCKET;
static bool initWsa() {
    WSAData d;
    return WSAStartup(MAKEWORD(2, 2), &d) == 0;
}
static bool wsaReady() {
    // WSA живёт до конца процесса, WSACleanup не нужен.
    static bool ok = initWsa();
    return ok;
}
static std::string netError(const char *what, int code) {
    return std::string(what) + ": WSA " + std::to_string(code);
}
// setsockopt хочет const char* на Winsock и const void* на POSIX.
static const char *sockOpt(const int *v) {
    return reinterpret_cast<const char *>(v);
}
#else
using TspHandle = int;
constexpr TspHandle kTspInvalid = -1;
static const void *sockOpt(const int *v) {
    return v;
}
#endif

class TspSocket {
public:
    static constexpr int kHeaderSize = 7;
    static constexpr int kMaxPayload = 4 * 1024 * 1024;

    TspSocket() = default;
    ~TspSocket() { close(); }

    TspSocket(const TspSocket &) = delete;
    TspSocket &operator=(const TspSocket &) = delete;

    TspSocket(TspSocket &&other) noexcept : fd_(other.fd_), error_(std::move(other.error_)), peer_(std::move(other.peer_)) {
        other.fd_ = kTspInvalid;
    }
    TspSocket &operator=(TspSocket &&other) noexcept {
        if (this != &other) {
            close();
            fd_ = other.fd_;
            error_ = std::move(other.error_);
            peer_ = std::move(other.peer_);
            other.fd_ = kTspInvalid;
        }
        return *this;
    }

    void close() {
        if (fd_ != kTspInvalid) {
#if defined(_WIN32)
            ::closesocket(fd_);
#else
            ::close(fd_);
#endif
            fd_ = kTspInvalid;
        }
    }

    bool isOpen() const { return fd_ != kTspInvalid; }
    TspHandle fd() const { return fd_; }
    // Path MTU коннекта (орига кладёт его в LoginMessage.RndKey).
    int pathMtu(int fallback = 1400) const {
        if (fd_ == kTspInvalid) {
            return fallback;
        }
#if defined(_WIN32)
        // IP_MTU нет в Winsock — отдаём фолбэк.
        return fallback;
#else
        int mtu = 0;
        socklen_t len = sizeof(mtu);
        if (getsockopt(fd_, IPPROTO_IP, IP_MTU, &mtu, &len) != 0 || mtu <= 0) {
            return fallback;
        }
        return mtu;
#endif
    }
    const std::string &lastError() const { return error_; }
    const std::string &peer() const { return peer_; }

    bool setBlocking(bool blocking) {
#if defined(_WIN32)
        u_long mode = blocking ? 0 : 1;
        return ioctlsocket(fd_, FIONBIO, &mode) == 0;
#else
        int flags = fcntl(fd_, F_GETFL, 0);
        if (flags < 0) {
            return false;
        }
        int next = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
        return fcntl(fd_, F_SETFL, next) == 0;
#endif
    }

    bool connect(const char *host, std::int32_t port, int timeoutMs = 5000) {
        close();
        error_.clear();
#if defined(_WIN32)
        if (!wsaReady()) {
            error_ = "WSAStartup failed";
            return false;
        }
#endif
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
#if defined(_WIN32)
        if (fd_ == kTspInvalid) {
            error_ = netError("socket", WSAGetLastError());
            return false;
        }
#else
        if (fd_ < 0) {
            error_ = "socket failed";
            return false;
        }
#endif
        if (!setBlocking(false)) {
#if defined(_WIN32)
            error_ = netError("nonblock", WSAGetLastError());
#else
            error_ = "nonblock failed";
#endif
            close();
            return false;
        }
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(static_cast<std::uint16_t>(port));
        if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
            error_ = "bad host";
            close();
            return false;
        }
        int rc = ::connect(fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
#if defined(_WIN32)
        if (rc != 0) {
            int e = WSAGetLastError();
            if (e != WSAEWOULDBLOCK) {
                error_ = netError("connect", e);
                close();
                return false;
            }
        }
#else
        if (rc < 0 && errno != EINPROGRESS) {
            error_ = std::string("connect: ") + std::strerror(errno);
            close();
            return false;
        }
#endif
        if (!waitWritable(timeoutMs)) {
            error_ = "connect timeout";
            close();
            return false;
        }
        setBlocking(true);
        int soErr = 0;
#if defined(_WIN32)
        int soLen = sizeof(soErr);
#else
        socklen_t soLen = sizeof(soErr);
#endif
        getsockopt(fd_, SOL_SOCKET, SO_ERROR,
#if defined(_WIN32)
            reinterpret_cast<char *>(&soErr),
#else
            &soErr,
#endif
            &soLen);
        if (soErr != 0) {
#if defined(_WIN32)
            error_ = netError("connect", soErr);
#else
            error_ = std::string("connect: ") + std::strerror(soErr);
#endif
            close();
            return false;
        }
        int one = 1;
        setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, sockOpt(&one), sizeof(one));
        setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, sockOpt(&one), sizeof(one));
        char ip[64];
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
        peer_ = std::string(ip) + ":" + std::to_string(port);
        TSPLOG("connect %s OK", peer_.c_str());
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
#if defined(_WIN32)
        int rc = select(0, nullptr, &wfds, nullptr, &tv);
#else
        int rc = select(fd_ + 1, nullptr, &wfds, nullptr, &tv);
#endif
        return rc > 0;
    }

    bool sendAll(const std::uint8_t *buf, int n) {
        int sent = 0;
        while (sent < n) {
#if defined(_WIN32)
            auto r = ::send(fd_, reinterpret_cast<const char *>(buf + sent), n - sent, 0);
#else
            auto r = ::send(fd_, buf + sent, static_cast<std::size_t>(n - sent), MSG_NOSIGNAL);
#endif
            if (r <= 0) {
#if defined(_WIN32)
                error_ = r == 0 ? "send closed" : netError("send", WSAGetLastError());
#else
                error_ = r == 0 ? "send closed" : std::string("send: ") + std::strerror(errno);
#endif
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
#if defined(_WIN32)
            int rc = select(0, &rfds, nullptr, nullptr, &tv);
#else
            int rc = select(fd_ + 1, &rfds, nullptr, nullptr, &tv);
#endif
            if (rc <= 0) {
                error_ = rc == 0 ? "recv timeout" : "select failed";
                return false;
            }
#if defined(_WIN32)
            auto r = ::recv(fd_, reinterpret_cast<char *>(buf + got), n - got, 0);
#else
            auto r = ::recv(fd_, buf + got, static_cast<std::size_t>(n - got), 0);
#endif
            if (r == 0) {
                error_ = "connection closed";
                return false;
            }
            if (r < 0) {
#if defined(_WIN32)
                int e = WSAGetLastError();
                if (e == WSAEINTR) {
                    continue;
                }
                error_ = netError("recv", e);
#else
                if (errno == EINTR) {
                    continue;
                }
                error_ = std::string("recv: ") + std::strerror(errno);
#endif
                return false;
            }
            got += static_cast<int>(r);
        }
        return true;
    }

    TspHandle fd_ = kTspInvalid;
    std::string error_;
    std::string peer_;
};
