#pragma once

// LoginUsingSessionMessage (10102) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LoginUsingSessionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10102; }
    const char* getMessageTypeName() const override { return "LoginUsingSessionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(f132_);
        stream().writeInt(f136_);
        for (auto* s : {&f144_, &f152_, &f160_, &f168_, &f176_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readInt();
        f136_ = stream().readInt();
        f144_ = stream().readString();
        f152_ = stream().readString();
        f160_ = stream().readString();
        f168_ = stream().readString();
        f176_ = stream().readString();
    }
    i32 f132_ = 0, f136_ = 0;
    std::optional<std::string> f144_, f152_, f160_, f168_, f176_;
};

} // namespace titan
