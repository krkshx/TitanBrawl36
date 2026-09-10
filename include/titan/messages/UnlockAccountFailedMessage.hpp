#pragma once

// UnlockAccountFailedMessage (20133) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class UnlockAccountFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20133; }
    const char* getMessageTypeName() const override { return "UnlockAccountFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
    }
    i32 reason_ = 0;
};

} // namespace titan
