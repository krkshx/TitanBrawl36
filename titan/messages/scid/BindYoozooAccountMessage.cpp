#pragma once

// BindYoozooAccountMessage (14266) — Bind*Account family (see Common.hpp).
// Split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/Common.cpp"

namespace titan {

class BindYoozooAccountMessage : public BindAccountMessage {
public:
    int getMessageType() const override { return 14266; }
    const char *getMessageTypeName() const override { return "BindYoozooAccountMessage"; }
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
