#pragma once

// RemoveAvatarStreamEntryMessage (14418) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RemoveAvatarStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14418; }
    const char* getMessageTypeName() const override { return "RemoveAvatarStreamEntryMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        entryId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        entryId_ = LogicLong::decode(stream());
    }
    LogicLong entryId_;
};

} // namespace titan
