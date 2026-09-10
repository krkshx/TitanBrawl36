#pragma once

// AdUpdateConversionValueMessage (40000) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AdUpdateConversionValueMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 40000; }
    const char* getMessageTypeName() const override { return "AdUpdateConversionValueMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(value_);
    }
    void decode() override {
        PiranhaMessage::decode();
        value_ = stream().readInt();
    }
    i32 value_ = 0;
};

} // namespace titan
