#pragma once
#include "../home/LogicClientHome.cpp"
#include "../../data/LogicDataSlot.cpp"
#include <string>
#include <vector>

class LogicClientAvatar {
public:
    std::int32_t high0 = 0;
    std::int32_t low0 = 0;
    std::int32_t high1 = 0;
    std::int32_t low1 = 0;
    std::int32_t high2 = 0;
    std::int32_t low2 = 0;
    std::string name;
    bool b0 = false;
    std::int32_t v0 = 0;
    std::vector<LogicDataSlot> slots[8];
    std::int32_t tail[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool ok = false;

    void decode(ByteStream &s) {
        LogicLong a, b, cc;
        s.readLong(&a);
        s.readLong(&b);
        s.readLong(&cc);
        high0 = a.high;
        low0 = a.low;
        high1 = b.high;
        low1 = b.low;
        high2 = cc.high;
        low2 = cc.low;
        name = s.readString(200);
        b0 = s.readBoolean();
        v0 = s.readVInt();
        for (int i = 0; i < 8; i++) {
            std::int32_t n = s.readVInt();
            for (std::int32_t j = 0; j < n; j++) {
                slots[i].push_back(LogicDataSlot::decode(s));
            }
        }
        for (int i = 0; i < 12; i++) {
            tail[i] = s.readVInt();
        }
        ok = true;
    }
};
