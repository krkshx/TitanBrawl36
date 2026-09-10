#pragma once

// DebugCommandMessage (14178) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class DebugCommandMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14178; }
    const char* getMessageTypeName() const override { return "DebugCommandMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(commandId_);
        stream().writeInt(param_);
    }
    void decode() override {
        PiranhaMessage::decode();
        commandId_ = stream().readInt();
        param_ = stream().readInt();
    }
    i32 commandId_ = 0, param_ = 0;
};

} // namespace titan
