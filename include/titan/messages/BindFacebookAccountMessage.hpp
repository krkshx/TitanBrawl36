#pragma once

// BindFacebookAccountMessage (14201) — Bind*Account family (see Common.hpp).
// Split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class BindFacebookAccountMessage : public BindAccountMessage {
public:
    int getMessageType() const override { return 14201; }
    const char *getMessageTypeName() const override { return "BindFacebookAccountMessage"; }
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
