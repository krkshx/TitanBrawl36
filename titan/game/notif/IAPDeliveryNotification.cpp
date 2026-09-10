#pragma once

// IAPDeliveryNotification::encode @0x5fca18, decode @0x2e5f7c,
// getNotificationType @0xa71144 (-> 86).
// Wire: BaseNotification fields, then vint @+48.
// Decode mirrors the same order.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class IAPDeliveryNotification : public BaseNotification {
public:
    IAPDeliveryNotification() { type_ = 86; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 86; }

    i32 v48_ = 0;
};

} // namespace titan
