#pragma once
#include "LogicRewardConfig.cpp"
#include "LogicRankRewardConfig.cpp"
#include <string>
#include <vector>

struct LogicRankedSeason {
    std::int32_t v0 = 0;
    std::string name;
    std::int32_t v2 = 0;
    std::int32_t v3 = 0;
    std::vector<LogicRewardConfig> rewards;
    std::vector<LogicRankRewardConfig> rankRewards;
    static LogicRankedSeason decode(ByteStream &s) {
        LogicRankedSeason e;
        e.v0 = s.readVInt();
        e.name = s.readString(900000);
        e.v2 = s.readVInt();
        e.v3 = s.readVInt();
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.rewards.push_back(LogicRewardConfig::decode(s));
        }
        n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.rankRewards.push_back(LogicRankRewardConfig::decode(s));
        }
        return e;
    }
};
