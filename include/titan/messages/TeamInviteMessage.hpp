#pragma once

// TeamInviteMessage (14365) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamInviteMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14365; }
    const char* getMessageTypeName() const override { return "TeamInviteMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(slotId_);
        stream().writeVInt(unk134_);
        stream().writeVInt(unk135_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotId_ = stream().readVInt();
        unk134_ = stream().readVInt();
        unk135_ = stream().readVInt();
    }
    i32 slotId_ = 0, unk134_ = 0, unk135_ = 0;
};

} // namespace titan
