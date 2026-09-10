#pragma once

// TeamInvitationMessage (24589) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/TeamInvitation.hpp"

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
