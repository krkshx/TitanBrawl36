#pragma once

// CreateAccountOkMessage (20101) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class CreateAccountOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20101; }
    const char* getMessageTypeName() const override { return "CreateAccountOkMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(accountIdHigh_);
        stream().writeInt(accountIdLow_);
        stream().writeString(passToken_ ? &*passToken_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountIdHigh_ = stream().readInt();
        accountIdLow_ = stream().readInt();
        passToken_ = stream().readString();
    }
    i32 accountIdHigh_ = 0, accountIdLow_ = 0;
    std::optional<std::string> passToken_;
};

} // namespace titan
