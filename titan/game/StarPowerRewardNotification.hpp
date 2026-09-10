#pragma once

// StarPowerRewardNotification::encode @0x537d08, decode @0x250f6c,
// getNotificationType @0xa1da1c (-> 84).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class StarPowerRewardNotification : public BaseNotification {
public:
    StarPowerRewardNotification() { type_ = 84; }

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
    [[nodiscard]] int notificationType() const override { return 84; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
