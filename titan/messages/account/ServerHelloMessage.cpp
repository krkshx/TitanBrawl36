#pragma once

// ServerHelloMessage (20100) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ServerHelloMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20100; }
    const char* getMessageTypeName() const override { return "ServerHelloMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (sessionKey_) {
            const auto& p = *sessionKey_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        sessionKey_ = stream().readBytesNullable();
    }
    std::optional<std::vector<u8>> sessionKey_;
};

} // namespace titan
