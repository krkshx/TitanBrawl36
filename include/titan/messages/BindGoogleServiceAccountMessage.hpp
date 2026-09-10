#pragma once

// BindGoogleServiceAccountMessage (14262) — Bind*Account family (see Common.hpp).
// Split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class BindGoogleServiceAccountMessage : public BindAccountMessage {
public:
    int getMessageType() const override { return 14262; }
    const char *getMessageTypeName() const override { return "BindGoogleServiceAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        blobCount_ = 0;
        hasVintTail_ = false;
        hasForce_ = true;
        encodeBind();
    }
    void decode() override {
        PiranhaMessage::decode();
        blobCount_ = 0;
        hasVintTail_ = false;
        hasForce_ = true;
        decodeBind();
    }
};

} // namespace titan
