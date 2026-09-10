#pragma once

// TriggerStartLatencyTestMessage (19003) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TriggerStartLatencyTestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19003; }
    const char* getMessageTypeName() const override { return "TriggerStartLatencyTestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
        stream().writeVInt(v3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
        v3_ = stream().readVInt();
    }
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
};

} // namespace titan
