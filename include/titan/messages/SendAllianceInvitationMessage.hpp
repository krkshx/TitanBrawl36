#pragma once

// SendAllianceInvitationMessage (14322) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SendAllianceInvitationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14322; }
    const char* getMessageTypeName() const override { return "SendAllianceInvitationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
    }
    LogicLong avatarId_;
};

} // namespace titan
