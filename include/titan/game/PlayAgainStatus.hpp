#pragma once

// PlayAgainStatus::encode @0x94bcfc.
// Static-style helper over an external LogicArrayList<LogicLong>*
// (the this-ptr is unused): vint count + LogicLong::encode each.
// Modelled as a value object holding the ids; decode mirrors the shape.

#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

#include <vector>

namespace titan {

class PlayAgainStatus : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(ids_.size()));
        for (const auto& id : ids_) id.encode(s);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        ids_.clear();
        for (i32 i = 0; i < n; ++i) ids_.push_back(LogicLong::decode(s));
    }

    std::vector<LogicLong> ids_;
};

} // namespace titan
