#pragma once

// NotificationMessage (20801) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/messages/pending/Notification.cpp"

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

// NotificationMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.


namespace titan {

inline void NotificationMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(notifType_);
    if (!payload_) throw pending_reverse("NotificationMessage needs Notification");
    payload_->encode(stream());
}

inline void NotificationMessage::decode() {
PiranhaMessage::decode();
    notifType_ = stream().readVInt();
    payload_ = std::make_unique<Notification>();
    payload_->decode(stream());
}


} // namespace titan
