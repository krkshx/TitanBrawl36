#pragma once

// SetAllianceCountryMessage (14299) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SetAllianceCountryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14299; }
    const char* getMessageTypeName() const override { return "SetAllianceCountryMessage"; }
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
