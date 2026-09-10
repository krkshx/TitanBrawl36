#pragma once

// CreateGameFailedMessage (20402) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class CreateGameFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20402; }
    const char* getMessageTypeName() const override { return "CreateGameFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(errorCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readInt();
    }
    i32 errorCode_ = 0;
};

} // namespace titan
