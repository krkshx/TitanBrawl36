#pragma once

// ChangePlayerMapNameMessage (12106) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ChangePlayerMapNameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12106; }
    const char* getMessageTypeName() const override { return "ChangePlayerMapNameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        stream().writeString(newName_ ? &*newName_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        newName_ = stream().readString();
    }
    LogicLong mapId_;
    std::optional<std::string> newName_;
};

} // namespace titan
