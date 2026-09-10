#pragma once

// GatchaDrop::encode @0x7509c0.
// Wire: vint @+4, dataref (nullable @+8), vint @+0, dataref @+16/@+24/@+32
// (nullable), vint @+40, vint @+44.
// Decode mirrors the same order (stream ctor used by LogicClientHome::decode).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>

namespace titan {

class GatchaDrop : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v4_);
        DataReference::encodeNullable(s, ref8_);
        s.writeVInt(v0_);
        DataReference::encodeNullable(s, ref16_);
        DataReference::encodeNullable(s, ref24_);
        DataReference::encodeNullable(s, ref32_);
        s.writeVInt(v40_);
        s.writeVInt(v44_);
    }
    void decode(ByteStream& s) override {
        v4_ = s.readVInt();
        ref8_ = DataReference::decodeNullable(s);
        v0_ = s.readVInt();
        ref16_ = DataReference::decodeNullable(s);
        ref24_ = DataReference::decodeNullable(s);
        ref32_ = DataReference::decodeNullable(s);
        v40_ = s.readVInt();
        v44_ = s.readVInt();
    }

    i32 v4_ = 0;                          // +4 (wired first)
    std::optional<DataReference> ref8_;   // +8
    i32 v0_ = 0;                          // +0
    std::optional<DataReference> ref16_;  // +16
    std::optional<DataReference> ref24_;  // +24
    std::optional<DataReference> ref32_;  // +32
    i32 v40_ = 0, v44_ = 0;               // +40, +44
};

} // namespace titan
