#pragma once
#include "../../titan/core/ByteStream.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>

struct ReleaseEntry {
    std::int32_t ref = 0;
    std::int32_t v1 = 0;
    std::int32_t v2 = 0;
    static ReleaseEntry decode(ByteStream &s) {
        ReleaseEntry e;
        e.ref = ByteStreamHelper::readDataReference(&s);
        e.v1 = s.readInt();
        e.v2 = s.readInt();
        return e;
    }
};
