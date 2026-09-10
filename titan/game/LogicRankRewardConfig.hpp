#pragma once

// LogicRankRewardConfig::encode @0x7e06d0 (const).
// Wire: vint @+8, vint @+12.
// Decode mirrors the same order (stream ctor used by LogicRankedSeason::decode).

#include "titan/messages/Nested.cpp"

namespace titan {

class LogicRankRewardConfig : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v8_);
        s.writeVInt(v12_);
    }
    void decode(ByteStream& s) override {
        v8_ = s.readVInt();
        v12_ = s.readVInt();
    }

    i32 v8_ = 0;
    i32 v12_ = 0;
};

} // namespace titan
