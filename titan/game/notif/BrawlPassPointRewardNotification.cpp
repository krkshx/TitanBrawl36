#pragma once

// BrawlPassPointRewardNotification::encode @0x667e58, decode @0x29ed94,
// getNotificationType @0xa0bd40 (-> 71).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class BrawlPassPointRewardNotification : public BaseNotification {
public:
    BrawlPassPointRewardNotification() { type_ = 71; }

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
    [[nodiscard]] int notificationType() const override { return 71; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
