#pragma once

// ClientInfoMessage (10177) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ClientInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10177; }
    const char* getMessageTypeName() const override { return "ClientInfoMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(info_ ? &*info_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        info_ = stream().readString();
    }
    std::optional<std::string> info_;
};

} // namespace titan
