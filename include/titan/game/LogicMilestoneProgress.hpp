#pragma once

// LogicMilestoneProgress::encode @0x6b920c.
// Wire: 3 × vint @+0..+8.
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.hpp"

namespace titan {

class LogicMilestoneProgress : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : v_) s.writeVInt(v);
    }
    void decode(ByteStream& s) override {
        for (i32& v : v_) v = s.readVInt();
    }

    i32 v_[3] = {};
};

} // namespace titan
