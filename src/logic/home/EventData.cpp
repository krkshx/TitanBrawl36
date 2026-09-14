#pragma once
#include "EventSlot.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include "../data/ChronosTextEntry.cpp"
#include "../data/ChronosFileEntry.cpp"
#include "../data/LogicGemOffer.cpp"
#include "../data/LogicBattlePlayerMap.cpp"
#include "../ranked/LogicRankedSeason.cpp"
#include <string>
#include <vector>

struct EventData {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    std::int32_t v2 = 0;
    std::int32_t v3 = 0;
    std::int32_t v4 = 0;
    std::int32_t ref = 0;
    std::int32_t v6 = 0;
    std::int32_t v7 = 0;
    std::string text;
    std::vector<std::int32_t> values;
    std::vector<std::int32_t> items;
    std::int32_t v8 = 0;
    std::int32_t v9 = 0;
    bool hasMap = false;
    LogicBattlePlayerMap battleMap;
    std::int32_t v10 = 0;
    bool hasRanked = false;
    LogicRankedSeason ranked;
    std::int32_t v11 = 0;
    std::int32_t v12 = 0;
    bool hasCh1 = false;
    ChronosTextEntry ch1;
    bool hasCh2 = false;
    ChronosTextEntry ch2;
    bool hasOffer = false;
    LogicGemOffer offer;
    std::vector<std::int32_t> tail;
    bool hasFile = false;
    ChronosFileEntry file;
    static EventData decode(ByteStream &s) {
        EventData e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        e.v2 = s.readVInt();
        e.v3 = s.readVInt();
        e.v4 = s.readVInt();
        e.ref = ByteStreamHelper::readDataReference(&s);
        e.v6 = s.readVInt();
        e.v7 = s.readVInt();
        e.text = s.readString(900000);
        for (int i = 0; i < 3; i++) {
            e.values.push_back(s.readVInt());
        }
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.items.push_back(s.readVInt());
        }
        e.v8 = s.readVInt();
        e.v9 = s.readVInt();
        if (s.readBoolean()) {
            e.hasMap = true;
            e.battleMap = LogicBattlePlayerMap::decodeBody(s);
        }
        e.v10 = s.readVInt();
        e.hasRanked = s.readBoolean();
        if (e.hasRanked) {
            e.ranked = LogicRankedSeason::decode(s);
        }
        e.v11 = s.readVInt();
        e.v12 = s.readVInt();
        e.hasCh1 = s.readBoolean();
        if (e.hasCh1) {
            e.ch1 = ChronosTextEntry::decode(s);
        }
        e.hasCh2 = s.readBoolean();
        if (e.hasCh2) {
            e.ch2 = ChronosTextEntry::decode(s);
        }
        e.hasOffer = s.readBoolean();
        if (e.hasOffer) {
            e.offer = LogicGemOffer::decode(s);
        }
        n = s.readVInt();
        if (n >= 0) {
            for (std::int32_t i = 0; i < n; i++) {
                e.tail.push_back(s.readVInt());
            }
        }
        e.hasFile = s.readBoolean();
        if (e.hasFile) {
            e.file = ChronosFileEntry::decode(s);
        }
        return e;
    }
};
