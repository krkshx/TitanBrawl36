#pragma once

// LogicRankedMatchResultDebugInfo::encode @0x8c31b0 (const).
// Wire: 6 × vint @+8..+28.
// Decode mirrors the same order (stream ctor used by
// LogicRankedMatchRoundState::decode).

#include "titan/messages/Nested.hpp"

namespace titan {

class LogicRankedMatchResultDebugInfo : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : v_) s.writeVInt(v);
    }
    void decode(ByteStream& s) override {
        for (i32& v : v_) v = s.readVInt();
    }

    i32 v_[6] = {}; // +8..+28
};

} // namespace titan
