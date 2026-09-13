#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include "../../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>

struct CooldownEntry {
    std::int32_t v0 = 0;
    std::int32_t ref = 0;
    std::int32_t v2 = 0;
    static CooldownEntry decode(ByteStream &s) {
        CooldownEntry e;
        e.v0 = s.readVInt();
        e.ref = ByteStreamHelper::readDataReference(&s);
        e.v2 = s.readVInt();
        return e;
    }
};
