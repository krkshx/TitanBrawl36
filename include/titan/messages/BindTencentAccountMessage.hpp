#pragma once

// BindTencentAccountMessage (14266) — Bind*Account family (see Common.hpp).
// Split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class BindTencentAccountMessage : public BindAccountMessage {
public:
    int getMessageType() const override { return 14266; }
    const char *getMessageTypeName() const override { return "BindTencentAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        blobCount_ = 0;
        hasVintTail_ = true;
        hasForce_ = false;
        encodeBind();
    }
    void decode() override {
        PiranhaMessage::decode();
        blobCount_ = 0;
        hasVintTail_ = true;
        hasForce_ = false;
        decodeBind();
    }
};

} // namespace titan
