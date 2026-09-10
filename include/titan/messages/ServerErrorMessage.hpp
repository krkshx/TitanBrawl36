#pragma once

// ServerErrorMessage (24115) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ServerErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24115; }
    const char* getMessageTypeName() const override { return "ServerErrorMessage"; }
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
