#pragma once

// AdjustPackageMessage (17338) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AdjustPackageMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17338; }
    const char* getMessageTypeName() const override { return "AdjustPackageMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(packageType_);
        if (payload_) {
            const auto& p = *payload_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        packageType_ = stream().readInt();
        payload_ = stream().readBytesNullable();
    }
    i32 packageType_ = 0;
    std::optional<std::vector<u8>> payload_;
};

} // namespace titan
