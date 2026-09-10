#pragma once

// BoxRewardNotification::encode @0x395d38, decode @0x26c984,
// getNotificationType @0x9cf79c (-> 64).
// Wire: BaseNotification fields, then vint @+48/+52/+56.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class BoxRewardNotification : public BaseNotification {
public:
    BoxRewardNotification() { type_ = 64; }

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
    [[nodiscard]] int notificationType() const override { return 64; }

    i32 v48_ = 0;
    i32 v52_ = 0;
    i32 v56_ = 0;
};

} // namespace titan
