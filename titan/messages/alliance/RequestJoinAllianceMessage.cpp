#pragma once

// RequestJoinAllianceMessage (14317) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RequestJoinAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14317; }
    const char* getMessageTypeName() const override { return "RequestJoinAllianceMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
        stream().writeString(message_ ? &*message_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
        message_ = stream().readString();
    }
    LogicLong allianceId_;
    std::optional<std::string> message_;
};

} // namespace titan
