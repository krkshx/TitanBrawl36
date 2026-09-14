#pragma once
#include "LogicCondition.cpp"
#include "../data/LogicGemOffer.cpp"

struct LogicRewardConfig {
    bool hasCondition = false;
    LogicCondition condition;
    bool hasOffer = false;
    LogicGemOffer offer;
    static LogicRewardConfig decode(ByteStream &s) {
        LogicRewardConfig e;
        e.hasCondition = s.readBoolean();
        if (e.hasCondition) {
            e.condition = LogicCondition::decode(s);
        }
        e.hasOffer = s.readBoolean();
        if (e.hasOffer) {
            e.offer = LogicGemOffer::decode(s);
        }
        return e;
    }
};
