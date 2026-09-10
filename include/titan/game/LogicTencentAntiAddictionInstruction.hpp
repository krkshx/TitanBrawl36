#pragma once

// LogicTencentAntiAddictionInstruction::encode @0x8f84e0 (const).
// Wire: vint @+8, 4 × writeStringReference of the EMBEDDED Strings
// @+16/+32/+48/+72 (vtable slot +0x20), vint @+64.
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.hpp"

#include <string>

namespace titan {

class LogicTencentAntiAddictionInstruction : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v8_);
        s.writeStringReference(s16_);
        s.writeStringReference(s32_);
        s.writeStringReference(s48_);
        s.writeVInt(v64_);
        s.writeStringReference(s72_);
    }
    void decode(ByteStream& s) override {
        v8_ = s.readVInt();
        s16_ = s.readStringReference();
        s32_ = s.readStringReference();
        s48_ = s.readStringReference();
        v64_ = s.readVInt();
        s72_ = s.readStringReference();
    }

    i32 v8_ = 0;
    std::string s16_, s32_, s48_;
    i32 v64_ = 0;
    std::string s72_;
};

} // namespace titan
