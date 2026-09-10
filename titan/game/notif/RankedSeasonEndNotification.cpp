#pragma once

// RankedSeasonEndNotification::encode @0x7691ec, decode @0x90cb2c,
// getNotificationType @0xa5173c (-> 68).
// Wire: BaseNotification fields, vint @+48/+52/+56,
//   bool + LogicGemOffer @+64 (nullable).

#include "titan/game/notif/BaseNotification.cpp"
#include "titan/game/shop/LogicGemOffer.cpp"

#include <memory>

namespace titan {

class RankedSeasonEndNotification : public BaseNotification {
public:
    RankedSeasonEndNotification() { type_ = 68; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
        s.writeVInt(v56_);
        s.writeBoolean(offer_ != nullptr);
        if (offer_) offer_->encode(s);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        v52_ = s.readVInt();
        v56_ = s.readVInt();
        if (s.readBoolean()) {
            offer_ = std::make_unique<LogicGemOffer>();
            offer_->decode(s);
        } else {
            offer_.reset();
        }
    }
    [[nodiscard]] int notificationType() const override { return 68; }

    i32 v48_ = 0, v52_ = 0, v56_ = 0;
    std::unique_ptr<LogicGemOffer> offer_; // +64 (nullable)
};

} // namespace titan
