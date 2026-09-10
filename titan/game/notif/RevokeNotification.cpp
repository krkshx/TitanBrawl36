#pragma once

// RevokeNotification::encode @0x231f3c, decode @0x97ba18,
// getNotificationType @0xa5bb58 (-> 85).
// Wire: BaseNotification fields, vint @+48/+52, writeLongLong @+64 (u64),
//   vint @+56, string (nullable String* @+72).

#include "titan/game/notif/BaseNotification.cpp"

#include <cstdint>
#include <optional>
#include <string>

namespace titan {

class RevokeNotification : public BaseNotification {
public:
    RevokeNotification() { type_ = 85; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
        s.writeLongLong(static_cast<i64>(ts_));
        s.writeVInt(v56_);
        s.writeString(s72_.has_value() ? &s72_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        v52_ = s.readVInt();
        ts_ = static_cast<u64>(s.readLongLong());
        v56_ = s.readVInt();
        s72_ = s.readString();
    }
    [[nodiscard]] int notificationType() const override { return 85; }

    i32 v48_ = 0, v52_ = 0;
    u64 ts_ = 0; // +64 (writeLongLong)
    i32 v56_ = 0;
    std::optional<std::string> s72_;
};

} // namespace titan
