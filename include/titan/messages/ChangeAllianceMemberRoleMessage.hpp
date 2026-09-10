#pragma once

// ChangeAllianceMemberRoleMessage (14306) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ChangeAllianceMemberRoleMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14306; }
    const char* getMessageTypeName() const override { return "ChangeAllianceMemberRoleMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
        stream().writeVInt(role_);
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
        role_ = stream().readVInt();
    }
    LogicLong avatarId_;
    i32 role_ = 0;
};

} // namespace titan
