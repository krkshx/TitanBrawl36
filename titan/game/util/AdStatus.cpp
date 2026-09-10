#pragma once

// AdStatus::encode @0x8d21fc, decode @0x566588.
// Wire: vint @+0, vint @+4, vint @+8.

#include "titan/messages/Nested.cpp"

namespace titan {

class AdStatus : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(a_);
        s.writeVInt(b_);
        s.writeVInt(c_);
    }
    void decode(ByteStream& s) override {
        a_ = s.readVInt();
        b_ = s.readVInt();
        c_ = s.readVInt();
    }

    i32 a_ = 0;
    i32 b_ = 0;
    i32 c_ = 0;
};

} // namespace titan
