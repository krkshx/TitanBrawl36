#pragma once

// ShutdownStartedMessage (20161) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ShutdownStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20161; }
    const char* getMessageTypeName() const override { return "ShutdownStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(seconds_);
    }
    void decode() override {
        PiranhaMessage::decode();
        seconds_ = stream().readInt();
    }
    i32 seconds_ = 0;
};

} // namespace titan
