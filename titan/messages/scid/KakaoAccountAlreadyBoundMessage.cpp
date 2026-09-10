#pragma once

// KakaoAccountAlreadyBoundMessage (24204) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class KakaoAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 24204; }
    const char* getMessageTypeName() const override { return "KakaoAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(false, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(false, false);
    }
};

} // namespace titan
