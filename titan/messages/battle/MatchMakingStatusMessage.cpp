#pragma once

// MatchMakingStatusMessage (20405) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class MatchMakingStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20405; }
    const char* getMessageTypeName() const override { return "MatchMakingStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : ints_) stream().writeInt(x);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : ints_) x = stream().readInt();
        flag_ = stream().readBoolean();
    }
    i32 ints_[5] = {};
    bool flag_ = false;
};

} // namespace titan
