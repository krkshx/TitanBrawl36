#pragma once

// MatchmakeFailedMessage (24108) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class MatchmakeFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24108; }
    const char* getMessageTypeName() const override { return "MatchmakeFailedMessage"; }
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
