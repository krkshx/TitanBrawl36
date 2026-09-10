#pragma once

// AvatarNameCheckResponseMessage (20300) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AvatarNameCheckResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20300; }
    const char* getMessageTypeName() const override { return "AvatarNameCheckResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(success_);
        stream().writeInt(errorCode_);
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        success_ = stream().readBoolean();
        errorCode_ = stream().readInt();
        name_ = stream().readString();
    }
    bool success_ = false;
    i32 errorCode_ = 0;
    std::optional<std::string> name_;
};

} // namespace titan
