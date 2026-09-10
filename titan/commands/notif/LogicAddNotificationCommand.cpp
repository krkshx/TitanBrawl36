#pragma once

// LogicAddNotificationCommand (206) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicAddNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 206; }
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    bool hasNested_ = false;
    i32 type_ = 0;
    std::unique_ptr<NestedEntry> object_; // concrete class pending (virtual-encode)
};

} // namespace titan

// LogicAddNotificationCommand bodies (206) — split out; unchanged.
// Wire: bool has; [vint type + notification object]. The concrete
// notification class is pending (virtual-encode in the binary), so
// present payloads throw loudly; absent ones round-trip.


namespace titan {

inline void LogicAddNotificationCommand::encode(ByteStream& s) const {
    LogicCommand::encode(s);
    s.writeBoolean(hasNested_);
    if (hasNested_) {
        s.writeVInt(type_);
        if (!object_) throw pending_reverse("206 needs Notification class");
        object_->encode(s);
    }
}

inline void LogicAddNotificationCommand::decode(ByteStream& s) {
    LogicCommand::decode(s);
    hasNested_ = s.readBoolean();
    if (hasNested_) {
        type_ = s.readVInt();
        throw pending_reverse("206 needs Notification class");
    }
}

} // namespace titan
