// FriendOnlineStatusEntryMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/FriendOnlineStatusEntryMessage.hpp"

namespace titan {

void FriendOnlineStatusEntryMessage::encode() {
    PiranhaMessage::encode();
    avatarId_.encode(stream());
    stream().writeBoolean(hasStatus_);
    if (hasStatus_) {
        if (!status_) throw pending_reverse("FriendOnlineStatusEntryMessage needs FriendOnlineStatus");
        status_->encode(stream());
    }
}

void FriendOnlineStatusEntryMessage::decode() {
    PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    hasStatus_ = stream().readBoolean();
    if (hasStatus_) {
        status_ = std::make_unique<FriendOnlineStatus>();
        status_->decode(stream());
    }
}


} // namespace titan
