#pragma once

// BrawlPassAutoCollectSeasonNotification::encode @0x945310, decode @0x2a0dc4
// (no dedicated getNotificationType — factory case 'E'/69 builds C2Ei).
// Wire: BaseNotification fields, LogicRewards @+56 (required),
//   vint @+48 last.

#include "titan/game/BaseNotification.hpp"
#include "titan/messages/pending/LogicRewards.hpp"

#include <memory>

namespace titan {

class BrawlPassAutoCollectSeasonNotification : public BaseNotification {
public:
    BrawlPassAutoCollectSeasonNotification() { type_ = 69; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        if (!rewards_) throw pending_reverse("BrawlPassAutoCollectSeasonNotification needs LogicRewards");
        rewards_->encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        rewards_ = std::make_unique<LogicRewards>();
        rewards_->decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 69; }

    std::unique_ptr<LogicRewards> rewards_; // +56 (required)
    i32 v48_ = 0;
};

} // namespace titan
