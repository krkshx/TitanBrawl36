// LogicAddNotificationCommand bodies (206) — split out; unchanged.
// Wire: bool has; [vint type + notification object]. The concrete
// notification class is pending (virtual-encode in the binary), so
// present payloads throw loudly; absent ones round-trip.

#include "titan/commands/LogicAddNotificationCommand.hpp"

namespace titan {

void LogicAddNotificationCommand::encode(ByteStream& s) const {
    LogicCommand::encode(s);
    s.writeBoolean(hasNested_);
    if (hasNested_) {
        s.writeVInt(type_);
        if (!object_) throw pending_reverse("206 needs Notification class");
        object_->encode(s);
    }
}

void LogicAddNotificationCommand::decode(ByteStream& s) {
    LogicCommand::decode(s);
    hasNested_ = s.readBoolean();
    if (hasNested_) {
        type_ = s.readVInt();
        throw pending_reverse("206 needs Notification class");
    }
}

} // namespace titan
