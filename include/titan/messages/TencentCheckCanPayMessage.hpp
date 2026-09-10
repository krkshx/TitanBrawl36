#pragma once

// TencentCheckCanPayMessage (14268) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TencentCheckCanPayMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14268; }
    const char* getMessageTypeName() const override { return "TencentCheckCanPayMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(value_);
    }
    void decode() override {
        PiranhaMessage::decode();
        value_ = stream().readVInt();
    }
    i32 value_ = 0;
};

} // namespace titan
