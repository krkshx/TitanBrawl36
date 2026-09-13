#pragma once
#include <cstdint>

struct ServerConfig {
    static const char *host() { return "93.123.84.82"; }
    static constexpr std::int32_t port() { return 9339; }
};
