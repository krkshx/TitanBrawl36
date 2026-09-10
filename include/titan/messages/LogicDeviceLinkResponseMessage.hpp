#pragma once

// LogicDeviceLinkResponseMessage (26005) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LogicDeviceLinkResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26005; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
        stream().writeStringReference(f168_);
        stream().writeInt(f156_);
        stream().writeInt(f160_);
        stream().writeInt(f152_);
        stream().writeStringReference(f192_);
        id_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
        f168_ = stream().readStringReference();
        f156_ = stream().readInt();
        f160_ = stream().readInt();
        f152_ = stream().readInt();
        f192_ = stream().readStringReference();
        id_ = LogicLong::decode(stream());
    }
    std::string f136_, f168_, f192_;
    i32 f156_ = 0, f160_ = 0, f152_ = 0;
    LogicLong id_;
};

} // namespace titan
