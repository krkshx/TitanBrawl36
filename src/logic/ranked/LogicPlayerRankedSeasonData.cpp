#pragma once
#include "LogicPlayerRewardData.cpp"

struct LogicPlayerRankedSeasonData {
    std::int32_t v[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<LogicPlayerRewardData> rewards;
    static LogicPlayerRankedSeasonData decode(ByteStream &s) {
        LogicPlayerRankedSeasonData e;
        for (int i = 0; i < 11; i++) {
            e.v[i] = s.readVInt();
        }
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.rewards.push_back(LogicPlayerRewardData::decode(s));
        }
        return e;
    }
};
