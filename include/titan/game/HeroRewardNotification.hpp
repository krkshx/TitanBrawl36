#pragma once

// HeroRewardNotification::encode @0x646f18, decode @0x5c3bfc,
// getNotificationType @0xa0c950 (-> 93).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class HeroRewardNotification : public BaseNotification {
public:
    HeroRewardNotification() { type_ = 93; }

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
    [[nodiscard]] int notificationType() const override { return 93; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
