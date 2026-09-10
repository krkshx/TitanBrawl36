#pragma once

// GetTokenFriendResultMessage (22089) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GetTokenFriendResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22089; }
    const char* getMessageTypeName() const override { return "GetTokenFriendResultMessage"; }
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
