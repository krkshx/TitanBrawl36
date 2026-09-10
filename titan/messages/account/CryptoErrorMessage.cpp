#pragma once

// CryptoErrorMessage (29997) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class CryptoErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29997; }
    const char* getMessageTypeName() const override { return "CryptoErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(errorCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readVInt();
    }
    i32 errorCode_ = 0;
};

} // namespace titan
