#pragma once
#include "QuestData.cpp"

inline QuestData QuestData::decode(ByteStream &s) {
    QuestData e;
    for (int i = 0; i < 10; i++) {
        e.v[i] = s.readVInt();
    }
    e.b0 = s.readBoolean();
    e.b1 = s.readBoolean();
    e.ref = ByteStreamHelper::readDataReference(&s);
    for (int i = 0; i < 4; i++) {
        e.w[i] = s.readVInt();
    }
    return e;
}

inline LogicQuests LogicQuests::decode(ByteStream &s) {
    LogicQuests e;
    std::int32_t n = s.readVInt();
    for (std::int32_t i = 0; i < n; i++) {
        e.quests.push_back(QuestData::decode(s));
    }
    return e;
}
