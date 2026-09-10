#pragma once

// PersonalBreakStartedMessage (20171) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class PersonalBreakStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20171; }
    const char* getMessageTypeName() const override { return "PersonalBreakStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(f132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readInt();
    }
    i32 f132_ = 0;
};

} // namespace titan
