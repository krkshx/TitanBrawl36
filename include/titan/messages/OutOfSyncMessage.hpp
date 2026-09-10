#pragma once

// OutOfSyncMessage (24104) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class OutOfSyncMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24104; }
    const char* getMessageTypeName() const override { return "OutOfSyncMessage"; }
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
