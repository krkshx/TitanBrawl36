#pragma once

// ProLeagueSeasonData::encode @0x71b258.
// Wire: vint @+0, vint @+4.
// Decode is the stream ctor used by LogicDailyData::decode: same reads
// in the same order (mirrored below).

#include "titan/messages/Nested.cpp"

namespace titan {

class ProLeagueSeasonData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(a_);
        s.writeVInt(b_);
    }
    void decode(ByteStream& s) override {
        a_ = s.readVInt();
        b_ = s.readVInt();
    }

    i32 a_ = 0;
    i32 b_ = 0;
};

} // namespace titan
