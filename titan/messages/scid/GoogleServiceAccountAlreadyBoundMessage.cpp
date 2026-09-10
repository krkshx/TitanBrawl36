#pragma once

// GoogleServiceAccountAlreadyBoundMessage (23494) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GoogleServiceAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 23494; }
    const char* getMessageTypeName() const override { return "GoogleServiceAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(true, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(true, false);
    }
};

} // namespace titan
