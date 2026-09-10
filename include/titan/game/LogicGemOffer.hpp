#pragma once

// LogicGemOffer::encode @0x93ed54.
// Wire: vint @+0, vint @+4, dataref (nullable LogicData* @+8), vint @+16.
// Decode is the stream ctor used by LogicOfferBundle::decode: same reads
// in the same order (mirrored below).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>

namespace titan {

class LogicGemOffer : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v1_);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v4_);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v1_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
        v4_ = s.readVInt();
    }

    i32 v0_ = 0;
    i32 v1_ = 0;
    std::optional<DataReference> ref_;
    i32 v4_ = 0;
};

} // namespace titan
