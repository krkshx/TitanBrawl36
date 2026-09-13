#pragma once
#include <cstdint>
#include <random>

static const std::uint8_t PEPPER_CLIENT_SECRET[32] = {
    183, 59, 48, 134, 52, 11, 39, 173, 205, 86, 212, 151, 215, 76, 28, 223,
    1, 205, 116, 35, 222, 208, 176, 185, 37, 109, 177, 6, 244, 108, 122, 26
};

inline void randombytes(std::uint8_t *buf, int len) {
    int n = len < 32 ? len : 32;
    for (int i = 0; i < n; i++) {
        buf[i] = PEPPER_CLIENT_SECRET[i];
    }
    for (int i = n; i < len; i++) {
        buf[i] = 0;
    }
}

inline void randomBytesTrue(std::uint8_t *buf, int len) {
    static thread_local std::random_device device;
    for (int i = 0; i < len; i++) {
        buf[i] = static_cast<std::uint8_t>(device() & 0xFFu);
    }
}

inline std::int32_t randomIntTrue() {
    static thread_local std::random_device device;
    return static_cast<std::int32_t>(device());
}
