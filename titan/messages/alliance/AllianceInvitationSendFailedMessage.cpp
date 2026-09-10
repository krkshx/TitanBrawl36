#pragma once

// AllianceInvitationSendFailedMessage (24321) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AllianceInvitationSendFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24321; }
    const char* getMessageTypeName() const override { return "AllianceInvitationSendFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
    }
    i32 reason_ = 0;
};

} // namespace titan
