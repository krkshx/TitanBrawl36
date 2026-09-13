#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include "../../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>
#include <vector>

struct QuestData {
    std::int32_t v[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool b0 = false;
    bool b1 = false;
    std::int32_t ref = 0;
    std::int32_t w[4] = {0, 0, 0, 0};
    static QuestData decode(ByteStream &s);
};

struct LogicQuests {
    std::vector<QuestData> quests;
    static LogicQuests decode(ByteStream &s);
};
