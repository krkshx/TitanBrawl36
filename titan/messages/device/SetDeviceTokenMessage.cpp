#pragma once

// SetDeviceTokenMessage (10113) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SetDeviceTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10113; }
    const char* getMessageTypeName() const override { return "SetDeviceTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (token_) {
            const auto& p = *token_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
        stream().writeInt(platform_);
    }
    void decode() override {
        PiranhaMessage::decode();
        token_ = stream().readBytesNullable();
        platform_ = stream().readInt();
    }
    std::optional<std::vector<u8>> token_;
    i32 platform_ = 0;
};

} // namespace titan
