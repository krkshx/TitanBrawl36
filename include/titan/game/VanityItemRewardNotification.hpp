#pragma once

// VanityItemRewardNotification::encode @0x648d1c, decode @0x91b658,
// getNotificationType @0x9db440 (-> 72).
// Wire: BaseNotification fields, then vint @+48.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class VanityItemRewardNotification : public BaseNotification {
public:
    VanityItemRewardNotification() { type_ = 72; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 72; }

    i32 v48_ = 0;
};

} // namespace titan
