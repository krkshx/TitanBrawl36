#pragma once

// CoinDoublerRewardNotification::encode @0x6c3a54, decode @0x63bb74,
// getNotificationType @0xa29570 (-> 88).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class CoinDoublerRewardNotification : public BaseNotification {
public:
    CoinDoublerRewardNotification() { type_ = 88; }

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
    [[nodiscard]] int notificationType() const override { return 88; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
