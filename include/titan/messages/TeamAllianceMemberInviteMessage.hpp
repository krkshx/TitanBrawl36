#pragma once

// TeamAllianceMemberInviteMessage (14370) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamAllianceMemberInviteMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14370; }
    const char* getMessageTypeName() const override { return "TeamAllianceMemberInviteMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(slotId_);
        stream().writeVInt(clubIdHi_);
        stream().writeVInt(clubIdLo_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotId_ = stream().readVInt();
        clubIdHi_ = stream().readVInt();
        clubIdLo_ = stream().readVInt();
    }
    i32 slotId_ = 0, clubIdHi_ = 0, clubIdLo_ = 0;
};

} // namespace titan
