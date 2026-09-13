#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>
#include <vector>

struct BrawlPassSeasonData {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    bool b2 = false;
    std::int32_t v3 = 0;
    bool b4 = false;
    bool hasBits0 = false;
    std::vector<std::int32_t> bits0;
    bool hasBits1 = false;
    std::vector<std::int32_t> bits1;
    static BrawlPassSeasonData decode(ByteStream &s) {
        BrawlPassSeasonData e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        e.b2 = s.readBoolean();
        e.v3 = s.readVInt();
        e.b4 = s.readBoolean();
        e.hasBits0 = s.readBoolean();
        if (e.hasBits0) {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                e.bits0.push_back(s.readInt());
            }
        }
        e.hasBits1 = s.readBoolean();
        if (e.hasBits1) {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                e.bits1.push_back(s.readInt());
            }
        }
        return e;
    }
};
