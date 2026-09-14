#pragma once
#include "../../titan/core/ByteStream.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>

struct GatchaDrop {
    std::int32_t v0 = 0;
    std::int32_t r1 = 0;
    std::int32_t v2 = 0;
    std::int32_t r3 = 0;
    std::int32_t r4 = 0;
    std::int32_t r5 = 0;
    std::int32_t v6 = 0;
    std::int32_t v7 = 0;
    static GatchaDrop decode(ByteStream &s) {
        GatchaDrop e;
        e.v0 = s.readVInt();
        e.r1 = ByteStreamHelper::readDataReference(&s);
        e.v2 = s.readVInt();
        e.r3 = ByteStreamHelper::readDataReference(&s);
        e.r4 = ByteStreamHelper::readDataReference(&s);
        e.r5 = ByteStreamHelper::readDataReference(&s);
        e.v6 = s.readVInt();
        e.v7 = s.readVInt();
        return e;
    }
};
