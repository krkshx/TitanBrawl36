#pragma once

// NotificationMessage (20801) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/Notification.hpp"

namespace titan {

class NotificationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20801; }
    const char* getMessageTypeName() const override { return "NotificationMessage"; }
    void encode() override;
    void decode() override;
    i32 notifType_ = 0;
    std::unique_ptr<Notification> payload_;
};

} // namespace titan
