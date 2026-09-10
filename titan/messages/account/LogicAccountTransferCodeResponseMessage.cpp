#pragma once

// LogicAccountTransferCodeResponseMessage (27002) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicAccountTransferCodeResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 27002; }
    const char* getMessageTypeName() const override { return "LogicAccountTransferCodeResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(code_);
        stream().writeInt(f156_);
        stream().writeInt(f152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        code_ = stream().readStringReference();
        f156_ = stream().readInt();
        f152_ = stream().readInt();
    }
    std::string code_;
    i32 f156_ = 0, f152_ = 0;
};

} // namespace titan
