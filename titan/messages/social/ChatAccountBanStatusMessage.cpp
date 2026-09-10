#pragma once

// ChatAccountBanStatusMessage (20118) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ChatAccountBanStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20118; }
    const char* getMessageTypeName() const override { return "ChatAccountBanStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(banStatus_);
    }
    void decode() override {
        PiranhaMessage::decode();
        banStatus_ = stream().readInt();
    }
    i32 banStatus_ = 0;
};

} // namespace titan
