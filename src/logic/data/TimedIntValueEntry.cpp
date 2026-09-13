#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>

struct TimedIntValueEntry {
    std::int32_t v[4] = {0, 0, 0, 0};
    static TimedIntValueEntry decode(ByteStream &s) {
        TimedIntValueEntry e;
        for (int i = 0; i < 4; i++) {
            e.v[i] = s.readVInt();
        }
        return e;
    }
};
