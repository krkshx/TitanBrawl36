#pragma once

// ForcedDrops::encode @0x664128.
// Wire: vint a, vint b, vint count + vint array (LogicArrayList<int> at +8).
// Decode is the stream ctor called from LogicDailyData::decode @0x4360ac:
// same reads in the same order (mirrored below).

#include "titan/messages/Nested.hpp"

#include <vector>

namespace titan {

class ForcedDrops : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(a_);
        s.writeVInt(b_);
        s.writeVInt(static_cast<i32>(ids_.size()));
        for (const i32 id : ids_) s.writeVInt(id);
    }
    void decode(ByteStream& s) override {
        a_ = s.readVInt();
        b_ = s.readVInt();
        const i32 n = s.readVInt();
        ids_.clear();
        for (i32 i = 0; i < n; ++i) ids_.push_back(s.readVInt());
    }

    i32 a_ = 0;
    i32 b_ = 0;
    std::vector<i32> ids_;
};

} // namespace titan
