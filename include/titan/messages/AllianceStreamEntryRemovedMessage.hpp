#pragma once

// AllianceStreamEntryRemovedMessage (24318) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AllianceStreamEntryRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24318; }
    const char* getMessageTypeName() const override { return "AllianceStreamEntryRemovedMessage"; }
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
