#pragma once

// ExtendedSetEncryptionMessage (20000) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ExtendedSetEncryptionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20000; }
    const char* getMessageTypeName() const override { return "ExtendedSetEncryptionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (key_) {
            const auto& p = *key_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
        stream().writeInt(tail_);
    }
    void decode() override {
        PiranhaMessage::decode();
        key_ = stream().readBytesNullable();
        tail_ = stream().readInt();
    }
    std::optional<std::vector<u8>> key_;
    i32 tail_ = 0;
};

} // namespace titan
