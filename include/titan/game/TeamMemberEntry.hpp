#pragma once

// TeamMemberEntry::encode @0x762a50.
// Wire: bool +0, writeLong @+8, dataref @+16/@+24 (nullable),
//   vint +32/+36/+40/+44, bool +1, vint +48/+52/+56/+60/+64,
//   PlayerDisplayData @+72 (required), dataref @+80/@+88 (nullable),
//   vint +96.
// Decode is default-ctor + decode (see TeamEntry::decode @0x7c1a40):
// same reads in the same order (mirrored below).

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>

namespace titan {

class TeamMemberEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeBoolean(b0_);
        id8_.encode(s); // == writeLong
        DataReference::encodeNullable(s, ref16_);
        DataReference::encodeNullable(s, ref24_);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        s.writeVInt(v40_);
        s.writeVInt(v44_);
        s.writeBoolean(b1_);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
        s.writeVInt(v56_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
        if (!display_) throw pending_reverse("TeamMemberEntry needs PlayerDisplayData");
        display_->encode(s);
        DataReference::encodeNullable(s, ref80_);
        DataReference::encodeNullable(s, ref88_);
        s.writeVInt(v96_);
    }
    void decode(ByteStream& s) override {
        b0_ = s.readBoolean();
        id8_ = LogicLong::decode(s);
        ref16_ = DataReference::decodeNullable(s);
        ref24_ = DataReference::decodeNullable(s);
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        v40_ = s.readVInt();
        v44_ = s.readVInt();
        b1_ = s.readBoolean();
        v48_ = s.readVInt();
        v52_ = s.readVInt();
        v56_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
        display_ = std::make_unique<PlayerDisplayData>();
        display_->decode(s);
        ref80_ = DataReference::decodeNullable(s);
        ref88_ = DataReference::decodeNullable(s);
        v96_ = s.readVInt();
    }

    bool b0_ = false; // +0
    LogicLong id8_; // +8 (writeLong)
    std::optional<DataReference> ref16_, ref24_;
    i32 v32_ = 0, v36_ = 0, v40_ = 0, v44_ = 0;
    bool b1_ = false; // +1
    i32 v48_ = 0, v52_ = 0, v56_ = 0, v60_ = 0, v64_ = 0;
    std::unique_ptr<PlayerDisplayData> display_; // +72 (required)
    std::optional<DataReference> ref80_, ref88_;
    i32 v96_ = 0;
};

} // namespace titan
