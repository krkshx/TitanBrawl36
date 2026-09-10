#pragma once

// GetAllianceInviteTokenResultMessage (23302) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GetAllianceInviteTokenResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23302; }
    const char* getMessageTypeName() const override { return "GetAllianceInviteTokenResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        token_ = stream().readString();
    }
    i32 result_ = 0;
    std::optional<std::string> token_;
};

} // namespace titan
