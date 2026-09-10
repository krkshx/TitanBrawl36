#pragma once

// LogicHeroUpgrades::encode @0x5d26c8.
// Wire: vint @+0, dataref (nullable @+8), dataref (nullable @+16).
// Decode is default-ctor + decode (see LogicPlayer::decode @0x89bacc):
// same reads in the same order (mirrored below).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>

namespace titan {

class LogicHeroUpgrades : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        DataReference::encodeNullable(s, ref8_);
        DataReference::encodeNullable(s, ref16_);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        ref8_ = DataReference::decodeNullable(s);
        ref16_ = DataReference::decodeNullable(s);
    }

    i32 v0_ = 0;
    std::optional<DataReference> ref8_, ref16_;
};

} // namespace titan
