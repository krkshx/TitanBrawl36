// OwnHomeDataMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.

#include "titan/messages/OwnHomeDataMessage.hpp"

namespace titan {

void OwnHomeDataMessage::encode() {
    PiranhaMessage::encode();
    if (!home_) throw pending_reverse("OwnHomeDataMessage needs LogicClientHome");
    home_->encode(stream());
    if (!avatar_) throw pending_reverse("OwnHomeDataMessage needs LogicClientAvatar");
    avatar_->encode(stream());
    stream().writeVInt(f152_);
}

void OwnHomeDataMessage::decode() {
    PiranhaMessage::decode();
    home_ = std::make_unique<LogicClientHome>();
    home_->decode(stream());
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    f152_ = stream().readVInt();
}


} // namespace titan
