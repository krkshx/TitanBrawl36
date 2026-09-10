#pragma once

#include "titan/core/ByteStream.hpp"

// LogicLong — 64-bit id split into two ints.
// LogicLong::encode @0x90c8d8: writeInt(high); writeInt(low).

namespace titan {

struct LogicLong {
    i32 high = 0;
    i32 low = 0;

    void encode(ByteStream& s) const {
        s.writeInt(high);
        s.writeInt(low);
    }
    static LogicLong decode(ByteStream& s) {
        LogicLong v;
        v.high = s.readInt();
        v.low = s.readInt();
        return v;
    }
    bool operator==(const LogicLong& o) const { return high == o.high && low == o.low; }
};

// ByteStreamHelper::encodeLogicLong @0x3e4e78:
// writeVInt(high); writeVInt(low). (The RankingEntry::encode decompile
// shows a phantom third arg — disassembly confirms X2 is untouched and
// the callee only reads stream + long.)
inline void encodeLogicLong(ByteStream& s, const LogicLong& id) {
    s.writeVInt(id.high);
    s.writeVInt(id.low);
}

} // namespace titan
