#pragma once

// TeamInvitationResponseMessage (14479) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamInvitationResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14479; }
    const char* getMessageTypeName() const override { return "TeamInvitationResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(response_);
        inviterId_.encode(stream());
        stream().writeBoolean(accepted_);
    }
    void decode() override {
        PiranhaMessage::decode();
        response_ = stream().readVInt();
        inviterId_ = LogicLong::decode(stream());
        accepted_ = stream().readBoolean();
    }
    i32 response_ = 0;
    LogicLong inviterId_;
    bool accepted_ = false;
};

} // namespace titan
