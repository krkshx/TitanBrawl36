#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>
#include <string>

struct ChronosFileEntry {
    std::string a;
    std::string b;
    static ChronosFileEntry decode(ByteStream &s) {
        ChronosFileEntry e;
        e.a = s.readString(900000);
        e.b = s.readString(900000);
        return e;
    }
};
