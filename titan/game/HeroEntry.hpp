#pragma once

// HeroEntry::encode @0x76f47c.
// Wire: dataref (nullable @+0), dataref (nullable @+8), vint @+16/+20/+24.
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.cpp"

#include <optional>

namespace titan {

class HeroEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        DataReference::encodeNullable(s, ref0_);
        DataReference::encodeNullable(s, ref8_);
        s.writeVInt(v16_);
        s.writeVInt(v20_);
        s.writeVInt(v24_);
    }
    void decode(ByteStream& s) override {
        ref0_ = DataReference::decodeNullable(s);
        ref8_ = DataReference::decodeNullable(s);
        v16_ = s.readVInt();
        v20_ = s.readVInt();
        v24_ = s.readVInt();
    }

    std::optional<DataReference> ref0_, ref8_;
    i32 v16_ = 0, v20_ = 0, v24_ = 0;
};

} // namespace titan
