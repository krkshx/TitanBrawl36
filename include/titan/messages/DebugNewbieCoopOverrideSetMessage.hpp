#pragma once

// DebugNewbieCoopOverrideSetMessage (24116) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class DebugNewbieCoopOverrideSetMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24116; }
    const char* getMessageTypeName() const override { return "DebugNewbieCoopOverrideSetMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(value_);
    }
    void decode() override {
        PiranhaMessage::decode();
        value_ = stream().readInt();
    }
    i32 value_ = 0;
};

} // namespace titan
