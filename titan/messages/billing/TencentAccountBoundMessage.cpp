#pragma once

// TencentAccountBoundMessage (24220) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TencentAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24220; }
    const char* getMessageTypeName() const override { return "TencentAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readInt();
    }
    i32 v132_ = 0;
};

} // namespace titan
