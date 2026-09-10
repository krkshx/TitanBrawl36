#pragma once

// TicketRewardNotification::encode @0x241ba0, decode @0x19c014,
// getNotificationType @0xa2a064 (-> 91).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class TicketRewardNotification : public BaseNotification {
public:
    TicketRewardNotification() { type_ = 91; }

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
    [[nodiscard]] int notificationType() const override { return 91; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
