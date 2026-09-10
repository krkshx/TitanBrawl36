#pragma once

// GamecenterAccountBoundMessage (26085) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GamecenterAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26085; }
    const char* getMessageTypeName() const override { return "GamecenterAccountBoundMessage"; }
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
