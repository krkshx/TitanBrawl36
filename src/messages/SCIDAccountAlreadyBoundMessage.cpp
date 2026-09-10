// SCIDAccountAlreadyBoundMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/SCIDAccountAlreadyBoundMessage.hpp"

namespace titan {

void SCIDAccountAlreadyBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(token_ ? &*token_ : nullptr);
    stream().writeBoolean(hasPlayerId_);
    if (hasPlayerId_) playerId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("SCIDAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
    stream().writeString(extra_ ? &*extra_ : nullptr);
}

void SCIDAccountAlreadyBoundMessage::decode() {
    PiranhaMessage::decode();
    token_ = stream().readString();
    hasPlayerId_ = stream().readBoolean();
    if (hasPlayerId_) playerId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    extra_ = stream().readString();
}


} // namespace titan
