#pragma once

// JoinAllianceUsingTokenMessage (17190) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class JoinAllianceUsingTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17190; }
    const char* getMessageTypeName() const override { return "JoinAllianceUsingTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
        token_ = stream().readString();
    }
    LogicLong allianceId_;
    std::optional<std::string> token_;
};

} // namespace titan
