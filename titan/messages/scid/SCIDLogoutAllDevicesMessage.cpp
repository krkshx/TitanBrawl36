#pragma once

// SCIDLogoutAllDevicesMessage (11736) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SCIDLogoutAllDevicesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 11736; }
    const char* getMessageTypeName() const override { return "SCIDLogoutAllDevicesMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCompressedString> token_;
};

} // namespace titan

// SCIDLogoutAllDevicesMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void SCIDLogoutAllDevicesMessage::encode() {
PiranhaMessage::encode();
    encodeCompressed(stream(), token_, "SCIDLogoutAllDevicesMessage needs LogicCompressedString");
}

inline void SCIDLogoutAllDevicesMessage::decode() {
PiranhaMessage::decode();
    decodeCompressed(stream(), token_);
}

} // namespace titan
