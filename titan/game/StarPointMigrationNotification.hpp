#pragma once

// StarPointMigrationNotification::encode @0x3aea2c, decode @0x6f7070,
// getNotificationType @0x9ec744 (-> 80).
// Wire: BaseNotification fields, then vint @+48.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class StarPointMigrationNotification : public BaseNotification {
public:
    StarPointMigrationNotification() { type_ = 80; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 80; }

    i32 v48_ = 0;
};

} // namespace titan
