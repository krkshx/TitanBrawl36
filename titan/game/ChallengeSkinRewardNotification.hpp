#pragma once

// ChallengeSkinRewardNotification::encode @0x930b20, decode @0x8648d0,
// getNotificationType @0x9c7b88 (-> 75).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class ChallengeSkinRewardNotification : public BaseNotification {
public:
    ChallengeSkinRewardNotification() { type_ = 75; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        v52_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 75; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
