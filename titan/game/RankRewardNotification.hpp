#pragma once

// RankRewardNotification::encode @0x5078f0, decode @0x8908c0,
// getNotificationType @0xa0e9c0 (-> 78).
// Wire: BaseNotification fields, then vint @+48/+52.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class RankRewardNotification : public BaseNotification {
public:
    RankRewardNotification() { type_ = 78; }

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
    [[nodiscard]] int notificationType() const override { return 78; }

    i32 v48_ = 0;
    i32 v52_ = 0;
};

} // namespace titan
