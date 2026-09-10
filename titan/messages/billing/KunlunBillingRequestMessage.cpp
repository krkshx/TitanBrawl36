#pragma once

// KunlunBillingRequestMessage (10159) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class KunlunBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10159; }
    const char* getMessageTypeName() const override { return "KunlunBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (const auto& s : fields_) stream().writeStringReference(s);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (auto& s : fields_) s = stream().readStringReference();
    }
    std::string fields_[4];
};

} // namespace titan
