#pragma once

// AllianceResponseMessage (24333) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AllianceResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24333; }
    const char* getMessageTypeName() const override { return "AllianceResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(responseType_);
        stream().writeVInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        responseType_ = stream().readVInt();
        reason_ = stream().readVInt();
    }
    i32 responseType_ = 0, reason_ = 0;
};

} // namespace titan
