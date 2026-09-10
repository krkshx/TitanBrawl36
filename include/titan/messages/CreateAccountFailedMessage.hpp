#pragma once

// CreateAccountFailedMessage (20101) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class CreateAccountFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20101; }
    const char* getMessageTypeName() const override { return "CreateAccountFailedMessage"; }
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
