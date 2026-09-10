#pragma once

// TeamClearInviteMessage (14367) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamClearInviteMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14367; }
    const char* getMessageTypeName() const override { return "TeamClearInviteMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        inviteId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        inviteId_ = LogicLong::decode(stream());
    }
    LogicLong inviteId_;
};

} // namespace titan
