#pragma once
#include "../../../titan/core/ByteStream.cpp"
#include <cstdint>
#include <string>

struct BaseNotification {
    std::int32_t type = 0;
    std::int32_t v0 = 0;
    bool b0 = false;
    std::int32_t v1 = 0;
    std::string text;
    static BaseNotification decode(ByteStream &s, std::int32_t type) {
        BaseNotification e;
        e.type = type;
        e.v0 = s.readInt();
        e.b0 = s.readBoolean();
        e.v1 = s.readInt();
        e.text = s.readString(900000);
        return e;
    }
};
