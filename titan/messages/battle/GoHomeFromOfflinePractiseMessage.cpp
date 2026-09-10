#pragma once

// GoHomeFromOfflinePractiseMessage (14109) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GoHomeFromOfflinePractiseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14109; }
    const char* getMessageTypeName() const override { return "GoHomeFromOfflinePractiseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(unknown_);
    }
    void decode() override {
        PiranhaMessage::decode();
        unknown_ = stream().readBoolean();
    }
    bool unknown_ = false;
};

} // namespace titan
