#pragma once

// TicketCompensationNotification::encode @0x70cd68, decode @0x63f0e8,
// getNotificationType @0xa0b4c8 (-> 74).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class TicketCompensationNotification : public BaseNotification {
public:
    TicketCompensationNotification() { type_ = 74; }

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
    [[nodiscard]] int notificationType() const override { return 74; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
