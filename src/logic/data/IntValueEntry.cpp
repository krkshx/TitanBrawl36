#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>

struct IntValueEntry {
    std::int32_t key = 0;
    std::int32_t value = 0;
    static IntValueEntry decode(ByteStream &s) {
        IntValueEntry e;
        e.key = s.readInt();
        e.value = s.readInt();
        return e;
    }
};
