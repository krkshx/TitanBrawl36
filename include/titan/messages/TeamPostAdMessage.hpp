#pragma once

// TeamPostAdMessage (14360) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamPostAdMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14360; }
    const char* getMessageTypeName() const override { return "TeamPostAdMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
    }
    i32 v132_ = 0;
};

} // namespace titan
