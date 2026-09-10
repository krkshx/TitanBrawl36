#pragma once

// DebugPlayerMapReviewResultOverrideSetMessage (22109) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class DebugPlayerMapReviewResultOverrideSetMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22109; }
    const char* getMessageTypeName() const override { return "DebugPlayerMapReviewResultOverrideSetMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(value_);
    }
    void decode() override {
        PiranhaMessage::decode();
        value_ = stream().readVInt();
    }
    i32 value_ = 0;
};

} // namespace titan
