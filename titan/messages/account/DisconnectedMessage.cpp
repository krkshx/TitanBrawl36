#pragma once

// DisconnectedMessage (25892) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class DisconnectedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 25892; }
    const char* getMessageTypeName() const override { return "DisconnectedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readVInt();
    }
    i32 reason_ = 0;
};

} // namespace titan
