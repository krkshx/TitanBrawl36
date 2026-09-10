#pragma once

// SetEncryptionMessage (20000) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SetEncryptionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20000; }
    const char* getMessageTypeName() const override { return "SetEncryptionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (key_) {
            const auto& p = *key_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        key_ = stream().readBytesNullable();
    }
    std::optional<std::vector<u8>> key_;
};

} // namespace titan
