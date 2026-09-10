#pragma once

// SinglePlayerMatchRequestMessage (14118) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SinglePlayerMatchRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14118; }
    const char* getMessageTypeName() const override { return "SinglePlayerMatchRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), ref136_);
        DataReference::encodeNullable(stream(), ref144_);
        stream().writeVInt(p152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        ref136_ = DataReference::decodeNullable(stream());
        ref144_ = DataReference::decodeNullable(stream());
        p152_ = stream().readVInt();
    }
    std::optional<DataReference> ref136_, ref144_;
    i32 p152_ = 0;
};

} // namespace titan
