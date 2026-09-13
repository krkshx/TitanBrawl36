#pragma once
#include <chrono>
#include <cstdint>
#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

class Clock {
public:
    static std::int64_t nowMs() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    }
    static void sleepMs(std::int32_t ms) {
#if defined(_WIN32)
        Sleep(static_cast<unsigned>(ms));
#else
        struct timespec ts;
        ts.tv_sec = ms / 1000;
        ts.tv_nsec = static_cast<long>((ms % 1000) * 1000000);
        nanosleep(&ts, nullptr);
#endif
    }
};
