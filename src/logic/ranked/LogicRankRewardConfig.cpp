#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>

struct LogicRankRewardConfig {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    static LogicRankRewardConfig decode(ByteStream &s) {
        LogicRankRewardConfig e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        return e;
    }
};
