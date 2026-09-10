#pragma once

// ChatToAllianceStreamMessage (14315) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ChatToAllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14315; }
    const char* getMessageTypeName() const override { return "ChatToAllianceStreamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(chat_ ? &*chat_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        chat_ = stream().readString();
    }
    std::optional<std::string> chat_;
};

} // namespace titan
