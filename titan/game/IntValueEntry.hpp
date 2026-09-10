#pragma once

// IntValueEntry::encode @0x6b0790.
// Wire: int @+0, int @+4. Decode is the stream ctor used by
// LogicDailyData::decode: same reads in the same order (mirrored below).

#include "titan/messages/Nested.cpp"

namespace titan {

class IntValueEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeInt(a_);
        s.writeInt(b_);
    }
    void decode(ByteStream& s) override {
        a_ = s.readInt();
        b_ = s.readInt();
    }

    i32 a_ = 0;
    i32 b_ = 0;
};

} // namespace titan
