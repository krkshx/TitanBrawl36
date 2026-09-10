#pragma once

// AttributionEventMessage (10321) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AttributionEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10321; }
    const char* getMessageTypeName() const override { return "AttributionEventMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (const auto& p : params_) stream().writeStringReference(p);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (auto& p : params_) p = stream().readStringReference();
        flag_ = stream().readBoolean();
    }
    std::string params_[9];
    bool flag_ = false;
};

} // namespace titan
