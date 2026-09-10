#pragma once

// TeamSetMemberReadyMessage (14355) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamSetMemberReadyMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14355; }
    const char* getMessageTypeName() const override { return "TeamSetMemberReadyMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(isReady_);
        stream().writeVInt(v136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        isReady_ = stream().readBoolean();
        v136_ = stream().readVInt();
    }
    bool isReady_ = false;
    i32 v136_ = 0;
};

} // namespace titan
