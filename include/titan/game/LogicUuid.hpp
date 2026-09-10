#pragma once

// LogicUuid::encode @0x29dc9c (const).
// Wire: 2 × writeVLong @+8/+16 (u64 halves).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.hpp"

#include <cstdint>

namespace titan {

class LogicUuid : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVLong(static_cast<i64>(hi_));
        s.writeVLong(static_cast<i64>(lo_));
    }
    void decode(ByteStream& s) override {
        hi_ = static_cast<u64>(s.readVLong());
        lo_ = static_cast<u64>(s.readVLong());
    }

    u64 hi_ = 0; // +8
    u64 lo_ = 0; // +16
};

} // namespace titan
