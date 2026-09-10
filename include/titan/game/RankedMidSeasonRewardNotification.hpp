#pragma once

// RankedMidSeasonRewardNotification::encode @0x719c1c, decode @0x6ef500,
// getNotificationType @0x99cc64 (-> 67).
// Wire: BaseNotification fields, vint @+48,
//   bool + LogicRewardConfig @+56 (nullable).

#include "titan/game/BaseNotification.hpp"
#include "titan/game/LogicRewardConfig.hpp"

#include <memory>

namespace titan {

class RankedMidSeasonRewardNotification : public BaseNotification {
public:
    RankedMidSeasonRewardNotification() { type_ = 67; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        s.writeBoolean(config_ != nullptr);
        if (config_) config_->encode(s);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        if (s.readBoolean()) {
            config_ = std::make_unique<LogicRewardConfig>();
            config_->decode(s);
        } else {
            config_.reset();
        }
    }
    [[nodiscard]] int notificationType() const override { return 67; }

    i32 v48_ = 0;
    std::unique_ptr<LogicRewardConfig> config_; // +56 (nullable)
};

} // namespace titan
