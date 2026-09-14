#pragma once
#include "../home/LogicClientHome.cpp"
#include "../data/LogicDataSlot.cpp"
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

    // getNameSetByUser = *(a1+192) @ 0x613d30, isTutorialState = *(a1+200)<2 @ 0x6c88ec.
    // tail[11] — 12-й VInt хвоста = *(a1+200).
    bool nameSetByUser() const { return b0; }
    bool tutorialState() const { return tail[11] < 2; }

    void decode(ByteStream &s) {
        LogicLong a, b, cc;
        // В либе (_ZN17LogicClientAvatar6decodeEP10ByteStream @ 0x6e6688):
        // 3x decodeLogicLong (пары VInt), имя (200), bool, Int (фикс! +216),
        // проверка кол-ва слотов == 8, 8 массивов слотов, 12x VInt.
        ByteStreamHelper::decodeLogicLong(&s, &a);
        ByteStreamHelper::decodeLogicLong(&s, &b);
        ByteStreamHelper::decodeLogicLong(&s, &cc);
        high0 = a.high;
        low0 = a.low;
        high1 = b.high;
        low1 = b.low;
        high2 = cc.high;
        low2 = cc.low;
        name = s.readString(200);
        b0 = s.readBoolean();
        v0 = s.readInt();
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
