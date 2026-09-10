// Message batch 09 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch09.hpp"

namespace titan {

// ---- 24123 SeasonRewardsMessage ----
void SeasonRewardsMessage::encode() {
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
void SeasonRewardsMessage::decode() {
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
