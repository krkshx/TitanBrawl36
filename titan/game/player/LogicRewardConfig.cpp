#pragma once

// LogicRewardConfig::encode @0x88d288, decode @0x710eb4.
// Wire: bool + LogicCondition @+8 (nullable, default-ctor + decode),
//   bool + LogicGemOffer @+16 (nullable, stream ctor).

#include "titan/game/util/LogicCondition.cpp"
#include "titan/game/shop/LogicGemOffer.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>

namespace titan {

class LogicRewardConfig : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeBoolean(cond_ != nullptr);
        if (cond_) cond_->encode(s);
        s.writeBoolean(offer_ != nullptr);
        if (offer_) offer_->encode(s);
    }
    void decode(ByteStream& s) override {
        if (s.readBoolean()) {
            cond_ = std::make_unique<LogicCondition>();
            cond_->decode(s);
        } else {
            cond_.reset();
        }
        if (s.readBoolean()) {
            offer_ = std::make_unique<LogicGemOffer>();
            offer_->decode(s);
        } else {
            offer_.reset();
        }
    }

    std::unique_ptr<LogicCondition> cond_;  // +8 (nullable)
    std::unique_ptr<LogicGemOffer> offer_;  // +16 (nullable)
};

} // namespace titan
