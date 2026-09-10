#pragma once

// VanityItemProp::encode @0x8bd8d0.
// Wire: vint @+0, vint @+4.
// Decode mirrors the same order (stream ctor used by VanityItemEntry::decode).

#include "titan/messages/Nested.cpp"

namespace titan {

class VanityItemProp : public NestedEntry {
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
