#pragma once

// TeamInviteResponseMessage (14368) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamInviteResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14368; }
    const char* getMessageTypeName() const override { return "TeamInviteResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(response_);
        stream().writeVInt(unk136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        response_ = stream().readVInt();
        unk136_ = stream().readVInt();
    }
    i32 response_ = 0, unk136_ = 0;
};

} // namespace titan
