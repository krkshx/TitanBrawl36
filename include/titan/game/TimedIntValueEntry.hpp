#pragma once

// TimedIntValueEntry::encode @0x6e4ee8.
// Wire: 4 × vint @+0..+12.
// Decode is the stream ctor used by LogicConfData::decode: same reads
// in the same order (mirrored below).

#include "titan/messages/Nested.hpp"

namespace titan {

class TimedIntValueEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : v_) s.writeVInt(v);
    }
    void decode(ByteStream& s) override {
        for (i32& v : v_) v = s.readVInt();
    }

    i32 v_[4] = {};
};

} // namespace titan
