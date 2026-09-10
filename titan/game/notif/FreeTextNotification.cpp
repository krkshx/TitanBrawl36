#pragma once

// FreeTextNotification::encode @0x2b1cf4, decode @0x8b24a8,
// getNotificationType @0x9fcd44 (-> 81, factory case 'Q' @0x5f60a4).
// Wire: BaseNotification fields, then vint @+48.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

class FreeTextNotification : public BaseNotification {
public:
    FreeTextNotification() { type_ = 81; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 81; }

    i32 v48_ = 0;
};

} // namespace titan
