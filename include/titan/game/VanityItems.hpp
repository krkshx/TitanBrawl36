#pragma once

// VanityItems::encode @0x5980fc.
// Wire: vint count + VanityItemEntry[] (LogicArrayList @+0, count +12).
// Decode is the stream ctor used by LogicDailyData::decode: same reads
// in the same order (mirrored below). Non-empty lists throw
// pending_reverse until VanityItemEntry lands.

#include "titan/messages/Nested.hpp"
#include "titan/messages/pending/VanityItemEntry.hpp"

#include <memory>
#include <vector>

namespace titan {

class VanityItems : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(items_.size()));
        for (const auto& e : items_) e->encode(s);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        items_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto e = std::make_unique<VanityItemEntry>();
            e->decode(s);
            items_.push_back(std::move(e));
        }
    }

    std::vector<std::unique_ptr<VanityItemEntry>> items_;
};

} // namespace titan
