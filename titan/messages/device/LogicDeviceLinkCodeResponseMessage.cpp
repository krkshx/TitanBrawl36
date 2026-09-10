#pragma once

// LogicDeviceLinkCodeResponseMessage (26002) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkCodeResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26002; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkCodeResponseMessage"; }
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
