#pragma once

// LogicCondition::encode @0x89d570 (const).
// Wire: vint @+8, vint @+12 (vtable @+0).
// Decode is default-ctor + decode (see LogicRewardConfig::decode @0x710eb4):
// same reads in the same order (mirrored below).

#include "titan/messages/Nested.cpp"

namespace titan {

class LogicCondition : public NestedEntry {
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
