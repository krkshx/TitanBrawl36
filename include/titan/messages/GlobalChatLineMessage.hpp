#pragma once

// GlobalChatLineMessage (24715) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GlobalChatLineMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24715; }
    const char* getMessageTypeName() const override { return "GlobalChatLineMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(message_ ? &*message_ : nullptr);
        stream().writeString(sender_ ? &*sender_ : nullptr);
        stream().writeVInt(senderLevel_);
        playerId_.encode(stream());
        targetId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        message_ = stream().readString();
        sender_ = stream().readString();
        senderLevel_ = stream().readVInt();
        playerId_ = LogicLong::decode(stream());
        targetId_ = LogicLong::decode(stream());
    }
    std::optional<std::string> message_, sender_;
    i32 senderLevel_ = 0;
    LogicLong playerId_, targetId_;
};

} // namespace titan
