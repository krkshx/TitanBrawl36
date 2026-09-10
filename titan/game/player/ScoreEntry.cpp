#pragma once

// ScoreEntry::encode @0x51d154.
// Wire: 4 × vint @+0..+12.
// Decode mirrors the same order (stream ctor used by
// StarPointsNotification::decode).

#include "titan/messages/Nested.cpp"

namespace titan {

class ScoreEntry : public NestedEntry {
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
