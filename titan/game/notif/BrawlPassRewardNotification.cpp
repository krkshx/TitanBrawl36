#pragma once

// BrawlPassRewardNotification::encode @0x8a2a8c, decode @0x696890,
// getNotificationType @0x9c0db4 (-> 73).
// Wire: BaseNotification fields, then vint @+48.
// Decode mirrors the same order.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class BrawlPassRewardNotification : public BaseNotification {
public:
    BrawlPassRewardNotification() { type_ = 73; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 73; }

    i32 v48_ = 0;
};

} // namespace titan
