#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>

struct VanityItemProp {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    static VanityItemProp decode(ByteStream &s) {
        VanityItemProp e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        return e;
    }
};
