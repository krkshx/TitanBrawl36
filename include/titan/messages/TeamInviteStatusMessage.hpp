#pragma once

// TeamInviteStatusMessage (24582) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/TeamInvitationDataEntry.hpp"

namespace titan {

class TeamInviteStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24582; }
    const char* getMessageTypeName() const override { return "TeamInviteStatusMessage"; }
    void encode() override;
    void decode() override;
    i32 statusCode_ = 0;
    std::unique_ptr<TeamInvitationDataEntry> entry_;
};

} // namespace titan
