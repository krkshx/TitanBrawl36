#pragma once

// LogicPlayer::encode @0x6bdfb0, decode @0x89bacc.
// Wire order (binary object offsets; note the odd order — kept as-is):
//   LogicLong id @+0, vint +8, vint +12, vint +84, int +16,
//   dataref @+56 (nullable), dataref @+24 (nullable),
//   bool + LogicHeroUpgrades @+168 (nullable),
//   bool + LogicBattleEmotes @+176 (nullable),
//   PlayerDisplayData @+280 (required, stream ctor), bool +296.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicBattleEmotes.hpp"
#include "titan/game/LogicHeroUpgrades.hpp"
#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>

namespace titan {

class LogicPlayer : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        s.writeVInt(v8_);
        s.writeVInt(v12_);
        s.writeVInt(v84_);
        s.writeInt(v16_);
        DataReference::encodeNullable(s, ref56_);
        DataReference::encodeNullable(s, ref24_);
        s.writeBoolean(upgrades_ != nullptr);
        if (upgrades_) upgrades_->encode(s);
        s.writeBoolean(emotes_ != nullptr);
        if (emotes_) emotes_->encode(s);
        if (!display_) throw pending_reverse("LogicPlayer needs PlayerDisplayData");
        display_->encode(s);
        s.writeBoolean(b296_);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        v8_ = s.readVInt();
        v12_ = s.readVInt();
        v84_ = s.readVInt();
        v16_ = s.readInt();
        ref56_ = DataReference::decodeNullable(s);
        ref24_ = DataReference::decodeNullable(s);
        if (s.readBoolean()) {
            upgrades_ = std::make_unique<LogicHeroUpgrades>();
            upgrades_->decode(s);
        } else {
            upgrades_.reset();
        }
        if (s.readBoolean()) {
            emotes_ = std::make_unique<LogicBattleEmotes>();
            emotes_->decode(s);
        } else {
            emotes_.reset();
        }
        display_ = std::make_unique<PlayerDisplayData>();
        display_->decode(s);
        b296_ = s.readBoolean();
    }

    LogicLong id_;                                   // +0
    i32 v8_ = 0, v12_ = 0;                           // +8, +12
    i32 v84_ = 0;                                    // +84 (wired early)
    i32 v16_ = 0;                                    // +16
    std::optional<DataReference> ref56_, ref24_;     // +56, +24
    std::unique_ptr<LogicHeroUpgrades> upgrades_;    // +168 (nullable)
    std::unique_ptr<LogicBattleEmotes> emotes_;      // +176 (nullable)
    std::unique_ptr<PlayerDisplayData> display_;     // +280 (required)
    bool b296_ = false;                              // +296
};

} // namespace titan
