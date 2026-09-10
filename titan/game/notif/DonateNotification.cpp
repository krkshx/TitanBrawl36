#pragma once

// DonateNotification::encode @0x40aa90, decode @0x669ee8,
// getNotificationType @0x99e874 (-> 2; no factory case — the binary
// Debugger::errors on receive, ours throws pending_reverse).
// Wire: BaseNotification fields, then string (nullable String* @+48).

#include "titan/game/notif/BaseNotification.cpp"

#include <optional>
#include <string>

namespace titan {

class DonateNotification : public BaseNotification {
public:
    DonateNotification() { type_ = 2; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeString(s48_.has_value() ? &s48_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        s48_ = s.readString();
    }
    [[nodiscard]] int notificationType() const override { return 2; }

    std::optional<std::string> s48_;
};

} // namespace titan
