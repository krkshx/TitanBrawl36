// NotificationMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.

#include "titan/messages/NotificationMessage.hpp"

namespace titan {

void NotificationMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(notifType_);
    if (!payload_) throw pending_reverse("NotificationMessage needs Notification");
    payload_->encode(stream());
}

void NotificationMessage::decode() {
    PiranhaMessage::decode();
    notifType_ = stream().readVInt();
    payload_ = std::make_unique<Notification>();
    payload_->decode(stream());
}


} // namespace titan
