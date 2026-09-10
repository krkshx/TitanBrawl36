#pragma once

// VanityItemEntry::encode @0x427c6c.
// Wire: dataref (LogicData* @+0, nullable), vint count + VanityItemProp[]
// (LogicArrayList @+8; a null list and an empty list both wire as vint 0).
// Decode mirrors the same order (stream ctor used by VanityItems::decode).

#include "titan/core/DataReference.hpp"
#include "titan/game/VanityItemProp.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <vector>

namespace titan {

class VanityItemEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(static_cast<i32>(props_.size()));
        for (const auto& p : props_) p->encode(s);
    }
    void decode(ByteStream& s) override {
        ref_ = DataReference::decodeNullable(s);
        const i32 n = s.readVInt();
        props_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto p = std::make_unique<VanityItemProp>();
            p->decode(s);
            props_.push_back(std::move(p));
        }
    }

    std::optional<DataReference> ref_;
    std::vector<std::unique_ptr<VanityItemProp>> props_;
};

} // namespace titan
