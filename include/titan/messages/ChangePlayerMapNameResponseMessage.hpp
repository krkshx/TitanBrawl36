#pragma once

// ChangePlayerMapNameResponseMessage (22106) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ChangePlayerMapNameResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22106; }
    const char* getMessageTypeName() const override { return "ChangePlayerMapNameResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(resultCode_);
        mapId_.encode(stream());
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        name_ = stream().readString();
    }
    i32 resultCode_ = 0;
    LogicLong mapId_;
    std::optional<std::string> name_;
};

} // namespace titan
