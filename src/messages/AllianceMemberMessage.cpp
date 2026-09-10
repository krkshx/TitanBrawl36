// AllianceMemberMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceMemberMessage.hpp"

namespace titan {

void AllianceMemberMessage::encode() {
    PiranhaMessage::encode();
    avatarId_.encode(stream());
    if (!entry_) throw pending_reverse("AllianceMemberMessage needs AllianceMemberEntry");
    entry_->encode(stream());
}

void AllianceMemberMessage::decode() {
    PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    entry_ = std::make_unique<AllianceMemberEntry>();
    entry_->decode(stream());
}


} // namespace titan
