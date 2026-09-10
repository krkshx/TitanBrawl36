#pragma once

// ReportUserStatusMessage (20117) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ReportUserStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20117; }
    const char* getMessageTypeName() const override { return "ReportUserStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(v1_);
        stream().writeInt(v2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readInt();
        v2_ = stream().readInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

} // namespace titan
