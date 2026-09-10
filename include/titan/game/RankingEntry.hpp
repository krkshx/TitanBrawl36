#pragma once

// RankingEntry::encode @0x770f68, decode @0x88e8d8.
// Wire: encodeLogicLong @+8 (vint pair), vint +16/+20,
//   bool + PlayerRankingData @+24 (nullable, default-ctor + virtual decode),
//   bool + AllianceRankingData @+32 (nullable, same).
// Decode mirrors the same order.

#include "titan/core/LogicLong.hpp"
#include "titan/game/AllianceRankingData.hpp"
#include "titan/game/PlayerRankingData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>

namespace titan {

class RankingEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        encodeLogicLong(s, id8_);
        s.writeVInt(v16_);
        s.writeVInt(v20_);
        s.writeBoolean(player_ != nullptr);
        if (player_) player_->encode(s);
        s.writeBoolean(alliance_ != nullptr);
        if (alliance_) alliance_->encode(s);
    }
    void decode(ByteStream& s) override {
        id8_ = decodeLogicLong(s);
        v16_ = s.readVInt();
        v20_ = s.readVInt();
        if (s.readBoolean()) {
            player_ = std::make_unique<PlayerRankingData>();
            player_->decode(s);
        } else {
            player_.reset();
        }
        if (s.readBoolean()) {
            alliance_ = std::make_unique<AllianceRankingData>();
            alliance_->decode(s);
        } else {
            alliance_.reset();
        }
    }

    LogicLong id8_; // +8 (vint pair)
    i32 v16_ = 0, v20_ = 0;
    std::unique_ptr<PlayerRankingData> player_; // +24 (nullable)
    std::unique_ptr<AllianceRankingData> alliance_; // +32 (nullable)
};

} // namespace titan
