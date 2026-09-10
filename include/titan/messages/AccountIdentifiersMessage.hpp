#pragma once

// AccountIdentifiersMessage (10111) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AccountIdentifiersMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10111; }
    const char* getMessageTypeName() const override { return "AccountIdentifiersMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(id_ ? &*id_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        id_ = stream().readString();
    }
    std::optional<std::string> id_;
};

} // namespace titan
