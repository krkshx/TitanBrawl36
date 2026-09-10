#pragma once

// BindGamecenterAccountMessage (14212) — Bind*Account family (see Common.hpp).
// Split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class BindGamecenterAccountMessage : public BindAccountMessage {
public:
    int getMessageType() const override { return 14212; }
    const char *getMessageTypeName() const override { return "BindGamecenterAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        blobCount_ = 3;
        hasVintTail_ = false;
        hasForce_ = true;
        encodeBind();
    }
    void decode() override {
        PiranhaMessage::decode();
        blobCount_ = 3;
        hasVintTail_ = false;
        hasForce_ = true;
        decodeBind();
    }
};

} // namespace titan
