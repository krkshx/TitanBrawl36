#pragma once

// SendGlobalChatLineMessage (14715) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SendGlobalChatLineMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14715; }
    const char* getMessageTypeName() const override { return "SendGlobalChatLineMessage"; }
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
