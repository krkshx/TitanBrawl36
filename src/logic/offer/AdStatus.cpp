#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>

struct AdStatus {
    std::int32_t v[3] = {0, 0, 0};
    static AdStatus decode(ByteStream &s) {
        AdStatus e;
        for (int i = 0; i < 3; i++) {
            e.v[i] = s.readVInt();
        }
        return e;
    }
};
