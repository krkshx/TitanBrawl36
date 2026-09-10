#pragma once

// SetCountryResponseMessage (24178) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetCountryResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24178; }
    const char* getMessageTypeName() const override { return "SetCountryResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        DataReference::encodeNullable(stream(), country_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        country_ = DataReference::decodeNullable(stream());
    }
    i32 result_ = 0;
    std::optional<DataReference> country_;
};

} // namespace titan
