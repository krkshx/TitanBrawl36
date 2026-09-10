#pragma once

// ClientCryptoErrorMessage (10099) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ClientCryptoErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10099; }
    const char* getMessageTypeName() const override { return "ClientCryptoErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(errorCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readInt();
    }
    i32 errorCode_ = 0;
};

} // namespace titan
