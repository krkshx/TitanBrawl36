#pragma once

// StartGameMessage (14103) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class StartGameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14103; }
    const char* getMessageTypeName() const override { return "StartGameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(p132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        p132_ = stream().readInt();
    }
    i32 p132_ = 0;
};

} // namespace titan
