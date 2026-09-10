#pragma once

// RewardEntry::encode @0x5515b4.
// Wire: vint +0/+4/+8, bool +32, LogicGemOffer @+16 (required),
//   bool + LogicGemOffer @+24 (nullable).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/game/LogicGemOffer.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>

namespace titan {

class RewardEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v4_);
        s.writeVInt(v8_);
        s.writeBoolean(b32_);
        if (!offer_) throw pending_reverse("RewardEntry needs LogicGemOffer");
        offer_->encode(s);
        s.writeBoolean(offer2_ != nullptr);
        if (offer2_) offer2_->encode(s);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v4_ = s.readVInt();
        v8_ = s.readVInt();
        b32_ = s.readBoolean();
        offer_ = std::make_unique<LogicGemOffer>();
        offer_->decode(s);
        if (s.readBoolean()) {
            offer2_ = std::make_unique<LogicGemOffer>();
            offer2_->decode(s);
        } else {
            offer2_.reset();
        }
    }

    i32 v0_ = 0, v4_ = 0, v8_ = 0;
    bool b32_ = false;
    std::unique_ptr<LogicGemOffer> offer_; // +16 (required)
    std::unique_ptr<LogicGemOffer> offer2_; // +24 (nullable)
};

} // namespace titan
