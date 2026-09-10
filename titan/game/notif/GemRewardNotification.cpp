#pragma once

// GemRewardNotification::encode @0x478458, decode @0x7bbd24,
// getNotificationType @0xa2b8a8 (-> 89).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class GemRewardNotification : public BaseNotification {
public:
    GemRewardNotification() { type_ = 89; }

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
    [[nodiscard]] int notificationType() const override { return 89; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
