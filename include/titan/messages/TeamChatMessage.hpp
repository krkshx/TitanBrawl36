#pragma once

// TeamChatMessage (14359) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamChatMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14359; }
    const char* getMessageTypeName() const override { return "TeamChatMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(text_ ? &*text_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        text_ = stream().readString();
    }
    std::optional<std::string> text_;
};

} // namespace titan
