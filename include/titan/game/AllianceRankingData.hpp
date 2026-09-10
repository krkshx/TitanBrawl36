#pragma once

// AllianceRankingData::encode @0x6ef580 (const), decode @0x1bf0d0.
// Wire: string (nullable String* @+8), vint @+16, dataref (nullable @+24).
// Decode mirrors the same order.

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

class AllianceRankingData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeString(name_.has_value() ? &name_.value() : nullptr);
        s.writeVInt(v16_);
        DataReference::encodeNullable(s, ref24_);
    }
    void decode(ByteStream& s) override {
        name_ = s.readString();
        v16_ = s.readVInt();
        ref24_ = DataReference::decodeNullable(s);
    }

    std::optional<std::string> name_; // +8
    i32 v16_ = 0;
    std::optional<DataReference> ref24_; // +24
};

} // namespace titan
