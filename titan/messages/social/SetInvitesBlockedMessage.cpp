#pragma once

// SetInvitesBlockedMessage (14777) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetInvitesBlockedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14777; }
    const char* getMessageTypeName() const override { return "SetInvitesBlockedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(blocked_);
    }
    void decode() override {
        PiranhaMessage::decode();
        blocked_ = stream().readBoolean();
    }
    bool blocked_ = false;
};

} // namespace titan
