#pragma once

// SetCountryMessage (12998) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetCountryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12998; }
    const char* getMessageTypeName() const override { return "SetCountryMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), country_);
    }
    void decode() override {
        PiranhaMessage::decode();
        country_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> country_;
};

} // namespace titan
