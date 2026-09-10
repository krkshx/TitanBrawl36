#pragma once

// ChangeAvatarNameMessage (10212) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ChangeAvatarNameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10212; }
    const char* getMessageTypeName() const override { return "ChangeAvatarNameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(newName_ ? &*newName_ : nullptr);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        newName_ = stream().readString();
        flag_ = stream().readBoolean();
    }
    std::optional<std::string> newName_;
    bool flag_ = false;
};

} // namespace titan
