#pragma once

// AvatarNameCheckRequestMessage (14600) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AvatarNameCheckRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14600; }
    const char* getMessageTypeName() const override { return "AvatarNameCheckRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        name_ = stream().readString();
    }
    std::optional<std::string> name_;
};

} // namespace titan
