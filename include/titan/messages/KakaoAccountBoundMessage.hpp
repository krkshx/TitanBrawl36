#pragma once

// KakaoAccountBoundMessage (24203) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class KakaoAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24203; }
    const char* getMessageTypeName() const override { return "KakaoAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readInt();
    }
    i32 resultCode_ = 0;
};

} // namespace titan
