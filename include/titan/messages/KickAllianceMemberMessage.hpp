#pragma once

// KickAllianceMemberMessage (14307) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class KickAllianceMemberMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14307; }
    const char* getMessageTypeName() const override { return "KickAllianceMemberMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        name_ = stream().readString();
    }
    LogicLong playerId_;
    std::optional<std::string> name_;
};

} // namespace titan
