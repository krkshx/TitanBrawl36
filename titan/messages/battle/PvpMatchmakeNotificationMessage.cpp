#pragma once

// PvpMatchmakeNotificationMessage (22957) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PvpMatchmakeNotificationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22957; }
    const char* getMessageTypeName() const override { return "PvpMatchmakeNotificationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(f132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readVInt();
    }
    i32 f132_ = 0;
};

} // namespace titan
