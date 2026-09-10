// FriendOnlineStatusMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/FriendOnlineStatusMessage.hpp"

namespace titan {

void FriendOnlineStatusMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(avatarIds_.size()));
    for (const auto& id : avatarIds_) id.encode(stream());
}

void FriendOnlineStatusMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    avatarIds_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) avatarIds_.push_back(LogicLong::decode(stream()));
}


} // namespace titan
