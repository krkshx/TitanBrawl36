#pragma once

// RequestLatencyTestStatusMessage (19004) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RequestLatencyTestStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19004; }
    const char* getMessageTypeName() const override { return "RequestLatencyTestStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
    }
    i32 v_ = 0;
};

} // namespace titan
