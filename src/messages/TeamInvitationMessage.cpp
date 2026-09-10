// TeamInvitationMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.

#include "titan/messages/TeamInvitationMessage.hpp"

namespace titan {

void TeamInvitationMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(slotId_);
    if (!invitation_) throw pending_reverse("TeamInvitationMessage needs TeamInvitation");
    invitation_->encode(stream());
}

void TeamInvitationMessage::decode() {
    PiranhaMessage::decode();
    slotId_ = stream().readVInt();
    invitation_ = std::make_unique<TeamInvitation>();
    invitation_->decode(stream());
}


} // namespace titan
