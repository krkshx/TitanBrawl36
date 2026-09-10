// LineAccountAlreadyBoundMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.

#include "titan/messages/LineAccountAlreadyBoundMessage.hpp"

namespace titan {

void LineAccountAlreadyBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(id_ ? &*id_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("LineAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
}

void LineAccountAlreadyBoundMessage::decode() {
    PiranhaMessage::decode();
    id_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
}


} // namespace titan
