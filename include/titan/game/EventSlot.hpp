#pragma once

// EventSlot::encode @0x587bb8.
// Wire: single vint @+0.
// Decode is the stream ctor used by LogicConfData::decode: same read
// (mirrored below).

#include "titan/messages/Nested.hpp"

namespace titan {

class EventSlot : public NestedEntry {
public:
    void encode(ByteStream& s) const override { s.writeVInt(v0_); }
    void decode(ByteStream& s) override { v0_ = s.readVInt(); }

    i32 v0_ = 0;
};

} // namespace titan
