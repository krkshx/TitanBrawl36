#pragma once

// TeamBotSlotDisableMessage (14373) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamBotSlotDisableMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14373; }
    const char* getMessageTypeName() const override { return "TeamBotSlotDisableMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(slotIndex_);
        stream().writeBoolean(disabled_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotIndex_ = stream().readInt();
        disabled_ = stream().readBoolean();
    }
    i32 slotIndex_ = 0;
    bool disabled_ = false;
};

} // namespace titan
