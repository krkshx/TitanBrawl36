#pragma once

// TeamInvitationMessage (24589) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/team/TeamInvitation.cpp"

namespace titan {

class TeamInvitationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24589; }
    const char* getMessageTypeName() const override { return "TeamInvitationMessage"; }
    void encode() override;
    void decode() override;
    i32 slotId_ = 0;
    std::unique_ptr<TeamInvitation> invitation_;
};

} // namespace titan

// TeamInvitationMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.


namespace titan {

inline void TeamInvitationMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(slotId_);
    if (!invitation_) throw pending_reverse("TeamInvitationMessage needs TeamInvitation");
    invitation_->encode(stream());
}

inline void TeamInvitationMessage::decode() {
PiranhaMessage::decode();
    slotId_ = stream().readVInt();
    invitation_ = std::make_unique<TeamInvitation>();
    invitation_->decode(stream());
}


} // namespace titan
