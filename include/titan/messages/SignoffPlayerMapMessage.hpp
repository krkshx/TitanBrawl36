#pragma once

// SignoffPlayerMapMessage (12111) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SignoffPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12111; }
    const char* getMessageTypeName() const override { return "SignoffPlayerMapMessage"; }
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
