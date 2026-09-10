#pragma once

// SkinRewardNotification::encode @0x72e720, decode @0x4610b4,
// getNotificationType @0x9e58bc (-> 94).
// Wire: BaseNotification fields, then vint @+48.
// Decode mirrors the same order.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class SkinRewardNotification : public BaseNotification {
public:
    SkinRewardNotification() { type_ = 94; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 94; }

    i32 v48_ = 0;
};

} // namespace titan
