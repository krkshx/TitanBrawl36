#pragma once

// PlayerJWTokenMessage (23774) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PlayerJWTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23774; }
    const char* getMessageTypeName() const override { return "PlayerJWTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        token_ = stream().readString();
    }
    std::optional<std::string> token_;
};

} // namespace titan
