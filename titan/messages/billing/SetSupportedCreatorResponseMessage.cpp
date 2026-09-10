#pragma once

// SetSupportedCreatorResponseMessage (28686) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetSupportedCreatorResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28686; }
    const char* getMessageTypeName() const override { return "SetSupportedCreatorResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeStringReference(code_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        code_ = stream().readStringReference();
    }
    i32 result_ = 0;
    std::string code_;
};

} // namespace titan
