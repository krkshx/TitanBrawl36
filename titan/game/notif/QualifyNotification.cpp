#pragma once

// QualifyNotification::getNotificationType @0x9acb0c (-> 76).
// No dedicated encode/decode symbols in the binary —
// inherits BaseNotification::encode/decode outright
// (factory case verified @0x5f5d50).

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class QualifyNotification : public BaseNotification {
public:
    QualifyNotification() { type_ = 76; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
    }
    [[nodiscard]] int notificationType() const override { return 76; }


};

} // namespace titan
