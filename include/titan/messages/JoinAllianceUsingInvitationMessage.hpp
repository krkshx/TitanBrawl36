#pragma once

// JoinAllianceUsingInvitationMessage (14323) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class JoinAllianceUsingInvitationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14323; }
    const char* getMessageTypeName() const override { return "JoinAllianceUsingInvitationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
    }
    LogicLong allianceId_;
};

} // namespace titan
