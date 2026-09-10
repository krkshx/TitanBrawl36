#pragma once

// TeamChangeMemberSettingsMessage (14354) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamChangeMemberSettingsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14354; }
    const char* getMessageTypeName() const override { return "TeamChangeMemberSettingsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), charRef_);
        DataReference::encodeNullable(stream(), skinRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        charRef_ = DataReference::decodeNullable(stream());
        skinRef_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> charRef_, skinRef_;
};

} // namespace titan
