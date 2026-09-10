#pragma once

// PlayerEntry::encode @0x7b5e20, decode @0x575470.
// Wire: bool +0/+1/+2, dataref @+8/@+16 (nullable), vint +24/+28/+32/+36,
//   bool + writeLong @+40 (nullable LogicLong*; writeLong @0x3f4e60 is
//   LogicLong::encode — the decompiler drops its second arg, verified in
//   disassembly: X1 still holds *(a1+40) at the BLR),
//   PlayerDisplayData @+48 (required, direct encode call).
// Decode reads the +40 long via readLong (read-vtable slot +272 = 0x110).

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/avatar/PlayerDisplayData.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <optional>

namespace titan {

class PlayerEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeBoolean(b0_);
        s.writeBoolean(b1_);
        s.writeBoolean(b2_);
        DataReference::encodeNullable(s, ref8_);
        DataReference::encodeNullable(s, ref16_);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        s.writeBoolean(opt40_.has_value());
        if (opt40_) opt40_->encode(s); // == writeLong @0x3f4e60
        if (!display_) throw pending_reverse("PlayerEntry needs PlayerDisplayData");
        display_->encode(s);
    }
    void decode(ByteStream& s) override {
        b0_ = s.readBoolean();
        b1_ = s.readBoolean();
        b2_ = s.readBoolean();
        ref8_ = DataReference::decodeNullable(s);
        ref16_ = DataReference::decodeNullable(s);
        v24_ = s.readVInt();
        v28_ = s.readVInt();
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        if (s.readBoolean()) {
            opt40_ = LogicLong::decode(s); // == readLong (slot +272)
        } else {
            opt40_.reset();
        }
        display_ = std::make_unique<PlayerDisplayData>();
        display_->decode(s);
    }

    bool b0_ = false, b1_ = false, b2_ = false;
    std::optional<DataReference> ref8_, ref16_;
    i32 v24_ = 0, v28_ = 0, v32_ = 0, v36_ = 0;
    std::optional<LogicLong> opt40_; // +40 (nullable long)
    std::unique_ptr<PlayerDisplayData> display_; // +48 (required)
};

} // namespace titan
