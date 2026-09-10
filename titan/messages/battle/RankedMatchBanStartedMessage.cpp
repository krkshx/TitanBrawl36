#pragma once

// RankedMatchBanStartedMessage (22151) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchBanStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22151; }
    const char* getMessageTypeName() const override { return "RankedMatchBanStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : v_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : v_) x = stream().readVInt();
    }
    i32 v_[3] = {};
};

} // namespace titan
