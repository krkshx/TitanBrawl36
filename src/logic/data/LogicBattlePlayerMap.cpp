#pragma once
#include "../../titan/core/ByteStream.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>
#include <string>
#include <vector>

struct LogicBattlePlayerMap {
    bool present = false;
    std::int32_t high0 = 0;
    std::int32_t low0 = 0;
    std::string name;
    std::int32_t v = 0;
    std::int32_t ref = 0;
    std::vector<std::uint8_t> compressed;
    std::int32_t high1 = 0;
    std::int32_t low1 = 0;
    std::string s2;
    std::int32_t v2 = 0;
    std::vector<std::int64_t> longs;
    static LogicBattlePlayerMap decode(ByteStream &s) {
        LogicBattlePlayerMap e;
        if (!s.readBoolean()) {
            return e;
        }
        e.present = true;
        return decodeBody(s);
    }
    static LogicBattlePlayerMap decodeBody(ByteStream &s) {
        LogicBattlePlayerMap e;
        e.present = true;
        LogicLong id;
        s.readLong(&id);
        e.high0 = id.high;
        e.low0 = id.low;
        e.name = s.readString(900000);
        e.v = s.readVInt();
        e.ref = ByteStreamHelper::readDataReference(&s);
        std::int32_t n = s.readBytesLength();
        e.compressed = s.readBytes(n, 900000);
        LogicLong id2;
        s.readLong(&id2);
        e.high1 = id2.high;
        e.low1 = id2.low;
        e.s2 = s.readString(900000);
        e.v2 = s.readVInt();
        n = s.readVInt();
        if (n >= 0) {
            for (std::int32_t i = 0; i < n; i++) {
                e.longs.push_back(s.readLongLong());
            }
        }
        return e;
    }
};
