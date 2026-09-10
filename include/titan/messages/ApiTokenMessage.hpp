#pragma once

// ApiTokenMessage (24758) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ApiTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24758; }
    const char* getMessageTypeName() const override { return "ApiTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(fA_);
        stream().writeString(token_ ? &*token_ : nullptr);
        stream().writeVInt(fB_);
    }
    void decode() override {
        PiranhaMessage::decode();
        fA_ = stream().readVInt();
        token_ = stream().readString();
        fB_ = stream().readVInt();
    }
    i32 fA_ = 0, fB_ = 0;
    std::optional<std::string> token_;
};

} // namespace titan
