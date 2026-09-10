#pragma once

// SendAllianceInvitationToFriendMessage (14326) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SendAllianceInvitationToFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14326; }
    const char* getMessageTypeName() const override { return "SendAllianceInvitationToFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
        stream().writeString(s144_ ? &*s144_ : nullptr);
        stream().writeString(s152_ ? &*s152_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
        s144_ = stream().readString();
        s152_ = stream().readString();
    }
    LogicLong avatarId_;
    std::optional<std::string> s144_, s152_;
};

} // namespace titan
