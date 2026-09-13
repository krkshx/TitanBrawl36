#pragma once
#include "../../data/LogicGemOffer.cpp"
#include <cstdint>

struct TimedOffer {
    std::int32_t ref = 0;
    std::int32_t v1 = 0;
    std::int32_t v2 = 0;
    static TimedOffer decode(ByteStream &s) {
        TimedOffer e;
        e.ref = ByteStreamHelper::readDataReference(&s);
        e.v1 = s.readVInt();
        e.v2 = s.readVInt();
        return e;
    }
};
