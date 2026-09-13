#pragma once
#include "LogicRewardConfig.cpp"
#include <vector>

struct LogicPlayerRewardData {
    bool hasConfig = false;
    LogicRewardConfig config;
    std::int32_t v = 0;
    bool flag = false;
    static LogicPlayerRewardData decode(ByteStream &s) {
        LogicPlayerRewardData e;
        e.hasConfig = s.readBoolean();
        if (e.hasConfig) {
            e.config = LogicRewardConfig::decode(s);
        }
        e.v = s.readVInt();
        e.flag = s.readBoolean();
        return e;
    }
};
