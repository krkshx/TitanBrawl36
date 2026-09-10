#pragma once

// RankedMatchFinalPreparationStartedMessage (22158) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class RankedMatchFinalPreparationStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22158; }
    const char* getMessageTypeName() const override { return "RankedMatchFinalPreparationStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
    }
    i32 v_ = 0;
};

} // namespace titan
