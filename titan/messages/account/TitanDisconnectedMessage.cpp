#pragma once

// TitanDisconnectedMessage (25892) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TitanDisconnectedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 25892; }
    const char* getMessageTypeName() const override { return "TitanDisconnectedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
    }
    i32 reason_ = 0;
};

} // namespace titan
