#pragma once

// StatusChangeEntry::encode @0x3af3e4.
// Wire: writeLong @+8 first, then vint @+0.
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

namespace titan {

class StatusChangeEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id8_.encode(s); // == writeLong
        s.writeVInt(v0_);
    }
    void decode(ByteStream& s) override {
        id8_ = LogicLong::decode(s);
        v0_ = s.readVInt();
    }

    LogicLong id8_; // +8 (wired first)
    i32 v0_ = 0; // +0
};

} // namespace titan
