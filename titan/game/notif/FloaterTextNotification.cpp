#pragma once

// FloaterTextNotification::getNotificationType @0xa4aa50 (-> 66).
// No dedicated encode/decode symbols in the binary —
// inherits BaseNotification::encode/decode outright
// (factory case verified @0x5f5d50).

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class FloaterTextNotification : public BaseNotification {
public:
    FloaterTextNotification() { type_ = 66; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
    }
    [[nodiscard]] int notificationType() const override { return 66; }


};

} // namespace titan
