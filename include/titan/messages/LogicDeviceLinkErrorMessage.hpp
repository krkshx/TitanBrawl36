#pragma once

// LogicDeviceLinkErrorMessage (26008) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LogicDeviceLinkErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26008; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(f132_);
        stream().writeInt(f136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readBoolean();
        f136_ = stream().readInt();
    }
    bool f132_ = false;
    i32 f136_ = 0;
};

} // namespace titan
