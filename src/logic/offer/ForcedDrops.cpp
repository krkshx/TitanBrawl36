#pragma once
#include "../../titan/core/ByteStream.cpp"
#include <cstdint>
#include <vector>

struct ForcedDrops {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    std::vector<std::int32_t> drops;
    static ForcedDrops decode(ByteStream &s) {
        ForcedDrops e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.drops.push_back(s.readVInt());
        }
        return e;
    }
};
