#pragma once

// LogicVector2::encode @0x29fda4 (const).
// Wire: 2 × writeInt @+0/+4 (NB: plain ints, not vints).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.cpp"

namespace titan {

class LogicVector2 : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeInt(x_);
        s.writeInt(y_);
    }
    void decode(ByteStream& s) override {
        x_ = s.readInt();
        y_ = s.readInt();
    }

    i32 x_ = 0;
    i32 y_ = 0;
};

} // namespace titan
