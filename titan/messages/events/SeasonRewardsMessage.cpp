#pragma once

// SeasonRewardsMessage (24123) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/LogicGemOffer.hpp"
#include "titan/game/RewardEntry.hpp"

namespace titan {

class SeasonRewardsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24123; }
    const char* getMessageTypeName() const override { return "SeasonRewardsMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0;
    std::vector<std::unique_ptr<RewardEntry>> rewards_;
    bool hasOffer_ = false;
    std::unique_ptr<LogicGemOffer> offer_;
};

} // namespace titan

// SeasonRewardsMessage bodies — split out of MsgBatch09.cpp; wire format unchanged.


namespace titan {

inline void SeasonRewardsMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeVInt(static_cast<i32>(rewards_.size()));
    for (const auto& r : rewards_) r->encode(stream());
    stream().writeBoolean(hasOffer_);
    if (hasOffer_) {
        if (!offer_) throw pending_reverse("SeasonRewardsMessage needs LogicGemOffer");
        offer_->encode(stream());
    }
}

inline void SeasonRewardsMessage::decode() {
PiranhaMessage::decode();
    v132_ = stream().readVInt();
    const i32 n = stream().readVInt();
    rewards_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto r = std::make_unique<RewardEntry>();
        r->decode(stream());
        rewards_.push_back(std::move(r));
    }
    hasOffer_ = stream().readBoolean();
    if (hasOffer_) {
        offer_ = std::make_unique<LogicGemOffer>();
        offer_->decode(stream());
    }
}

} // namespace titan
