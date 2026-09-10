#pragma once

// FacebookAccountBoundMessage (24201) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class FacebookAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24201; }
    const char* getMessageTypeName() const override { return "FacebookAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readInt();
    }
    i32 result_ = 0;
};

} // namespace titan
