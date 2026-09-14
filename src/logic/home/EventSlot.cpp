#pragma once
#include "../../titan/core/ByteStream.cpp"
#include <cstdint>

struct EventSlot {
    std::int32_t v = 0;
    static EventSlot decode(ByteStream &s) {
        EventSlot e;
        e.v = s.readVInt();
        return e;
    }
};
