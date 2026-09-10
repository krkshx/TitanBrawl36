#pragma once

// UdpConnectionInfoMessage (24112) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UdpConnectionInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24112; }
    const char* getMessageTypeName() const override { return "UdpConnectionInfoMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeString(addr_ ? &*addr_ : nullptr);
        if (key1_) {
            const auto& p = *key1_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
        if (key2_) {
            const auto& p = *key2_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        addr_ = stream().readString();
        key1_ = stream().readBytesNullable();
        key2_ = stream().readBytesNullable();
    }
    i32 v1_ = 0;
    std::optional<std::string> addr_;
    std::optional<std::vector<u8>> key1_, key2_;
};

} // namespace titan
