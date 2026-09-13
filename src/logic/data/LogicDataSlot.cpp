#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include "../../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>

struct LogicDataSlot {
    std::int32_t ref = 0;
    std::int32_t count = 0;
    static LogicDataSlot decode(ByteStream &s) {
        LogicDataSlot e;
        e.ref = ByteStreamHelper::readDataReference(&s);
        e.count = s.readVInt();
        return e;
    }
};
