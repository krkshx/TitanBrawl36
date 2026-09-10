#pragma once

// SetSupportedCreatorMessage (18686) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetSupportedCreatorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 18686; }
    const char* getMessageTypeName() const override { return "SetSupportedCreatorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(code_);
    }
    void decode() override {
        PiranhaMessage::decode();
        code_ = stream().readStringReference();
    }
    std::string code_;
};

} // namespace titan
