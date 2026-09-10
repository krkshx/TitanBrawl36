#pragma once

// TeamInviteStatusMessage (24582) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
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

// TeamInviteStatusMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.


namespace titan {

inline void TeamInviteStatusMessage::encode() {
PiranhaMessage::encode();
    stream().writeInt(statusCode_);
    stream().writeBoolean(entry_ != nullptr);
    if (entry_) entry_->encode(stream());
}

inline void TeamInviteStatusMessage::decode() {
PiranhaMessage::decode();
    statusCode_ = stream().readInt();
    if (stream().readBoolean()) {
        entry_ = std::make_unique<TeamInvitationDataEntry>();
        entry_->decode(stream());
    }
}

} // namespace titan
