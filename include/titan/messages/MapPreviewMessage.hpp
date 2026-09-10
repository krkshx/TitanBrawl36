#pragma once

// MapPreviewMessage (22202) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class MapPreviewMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22202; }
    const char* getMessageTypeName() const override { return "MapPreviewMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), mapRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapRef_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> mapRef_;
};

} // namespace titan
