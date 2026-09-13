#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>
#include <string>

struct ChronosTextEntry {
    std::int32_t id = -1;
    std::string text;
    static ChronosTextEntry decode(ByteStream &s) {
        ChronosTextEntry e;
        e.id = s.readInt();
        e.text = s.readString(900000);
        return e;
    }
};
