#pragma once

// HeroPowerRewardNotification::encode @0x49edac, decode @0x1a697c,
// getNotificationType @0x994544 (-> 92).
// Wire: BaseNotification fields, then vint @+48/+52/+56.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class HeroPowerRewardNotification : public BaseNotification {
public:
    HeroPowerRewardNotification() { type_ = 92; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
        s.writeVInt(v56_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        v52_ = s.readVInt();
        v56_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 92; }

    i32 v48_ = 0;
    i32 v52_ = 0;
    i32 v56_ = 0;
};

} // namespace titan
