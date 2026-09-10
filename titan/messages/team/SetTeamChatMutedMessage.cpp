#pragma once

// SetTeamChatMutedMessage (14778) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetTeamChatMutedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14778; }
    const char* getMessageTypeName() const override { return "SetTeamChatMutedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(muted_);
    }
    void decode() override {
        PiranhaMessage::decode();
        muted_ = stream().readBoolean();
    }
    bool muted_ = false;
};

} // namespace titan
