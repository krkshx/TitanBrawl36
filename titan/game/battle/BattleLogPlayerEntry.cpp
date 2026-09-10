#pragma once

// BattleLogPlayerEntry::encode @0x93cea4 (const).
// Wire: vint +0, writeLong @+8, vint +16, bool +20, dataref (nullable @+24),
//   vint +32/+36/+40, PlayerDisplayData @+48 (required).
// Decode mirrors the same order (stream ctor used by BattleLogEntry::decode).

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/avatar/PlayerDisplayData.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <optional>

namespace titan {

class BattleLogPlayerEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        id8_.encode(s); // == writeLong
        s.writeVInt(v16_);
        s.writeBoolean(b20_);
        DataReference::encodeNullable(s, ref24_);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        s.writeVInt(v40_);
        if (!display_) throw pending_reverse("BattleLogPlayerEntry needs PlayerDisplayData");
        display_->encode(s);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        id8_ = LogicLong::decode(s);
        v16_ = s.readVInt();
        b20_ = s.readBoolean();
        ref24_ = DataReference::decodeNullable(s);
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        v40_ = s.readVInt();
        display_ = std::make_unique<PlayerDisplayData>();
        display_->decode(s);
    }

    i32 v0_ = 0;
    LogicLong id8_; // +8 (writeLong)
    i32 v16_ = 0;
    bool b20_ = false;
    std::optional<DataReference> ref24_;
    i32 v32_ = 0, v36_ = 0, v40_ = 0;
    std::unique_ptr<PlayerDisplayData> display_; // +48 (required)
};

} // namespace titan
