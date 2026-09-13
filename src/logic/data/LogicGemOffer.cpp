#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include "../../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>

struct LogicGemOffer {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    std::int32_t ref = 0;
    std::int32_t v3 = 0;
    static LogicGemOffer decode(ByteStream &s) {
        LogicGemOffer o;
        o.v0 = s.readVInt();
        o.v1 = s.readVInt();
        o.ref = ByteStreamHelper::readDataReference(&s);
        o.v3 = s.readVInt();
        return o;
    }
};
