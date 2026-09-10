#pragma once

// TeamStreamEntryRemovedMessage (24319) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamStreamEntryRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24319; }
    const char* getMessageTypeName() const override { return "TeamStreamEntryRemovedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        id1_.encode(stream());
        id2_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        id1_ = LogicLong::decode(stream());
        id2_ = LogicLong::decode(stream());
    }
    LogicLong id1_, id2_;
};

} // namespace titan
