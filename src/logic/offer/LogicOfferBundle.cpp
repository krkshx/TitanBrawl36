#pragma once
#include "../../data/LogicGemOffer.cpp"
#include "../../data/ChronosTextEntry.cpp"
#include <cstdint>
#include <string>
#include <vector>

struct LogicOfferBundle {
    std::vector<LogicGemOffer> offers;
    std::int32_t v[5] = {0, 0, 0, 0, 0};
    bool b0 = false;
    std::int32_t v5 = 0;
    bool b1 = false;
    ChronosTextEntry chronos;
    bool b2 = false;
    std::string tag;
    std::int32_t v6 = 0;
    bool b3 = false;
    std::int32_t v7 = 0;
    std::int32_t v8 = 0;
    static LogicOfferBundle decode(ByteStream &s) {
        LogicOfferBundle e;
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.offers.push_back(LogicGemOffer::decode(s));
        }
        for (int i = 0; i < 5; i++) {
            e.v[i] = s.readVInt();
        }
        e.b0 = s.readBoolean();
        e.v5 = s.readVInt();
        e.b1 = s.readBoolean();
        e.chronos = ChronosTextEntry::decode(s);
        e.b2 = s.readBoolean();
        e.tag = s.readString(900000);
        e.v6 = s.readVInt();
        e.b3 = s.readBoolean();
        e.v7 = s.readVInt();
        e.v8 = s.readVInt();
        return e;
    }
};
