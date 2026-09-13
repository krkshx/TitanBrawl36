#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>

struct ProLeagueSeasonData {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    static ProLeagueSeasonData decode(ByteStream &s) {
        ProLeagueSeasonData e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        return e;
    }
};
