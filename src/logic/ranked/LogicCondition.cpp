#pragma once
#include "../../titan/core/ByteStream.cpp"
#include <cstdint>

struct LogicCondition {
    std::int32_t v0 = 0;
    std::int32_t v1 = 0;
    static LogicCondition decode(ByteStream &s) {
        LogicCondition e;
        e.v0 = s.readVInt();
        e.v1 = s.readVInt();
        return e;
    }
};
