#pragma once

// LineAccountBoundMessage (24206) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LineAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24206; }
    const char* getMessageTypeName() const override { return "LineAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readInt();
    }
    i32 result_ = 0;
};

} // namespace titan
