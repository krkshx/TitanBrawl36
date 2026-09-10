// FriendListUpdateMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/FriendListUpdateMessage.hpp"

namespace titan {

void FriendListUpdateMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    if (!entry_) throw pending_reverse("FriendListUpdateMessage needs FriendEntry");
    entry_->encode(stream());
}

void FriendListUpdateMessage::decode() {
    PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    entry_ = std::make_unique<FriendEntry>();
    entry_->decode(stream());
}


} // namespace titan
