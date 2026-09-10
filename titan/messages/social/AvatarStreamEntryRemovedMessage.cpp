#pragma once

// AvatarStreamEntryRemovedMessage (24418) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AvatarStreamEntryRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24418; }
    const char* getMessageTypeName() const override { return "AvatarStreamEntryRemovedMessage"; }
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
