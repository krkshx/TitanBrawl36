#pragma once

// AvatarNameChangeFailedMessage (20205) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AvatarNameChangeFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20205; }
    const char* getMessageTypeName() const override { return "AvatarNameChangeFailedMessage"; }
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
