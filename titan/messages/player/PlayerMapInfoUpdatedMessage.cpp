#pragma once

// PlayerMapInfoUpdatedMessage (22107) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PlayerMapInfoUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22107; }
    const char* getMessageTypeName() const override { return "PlayerMapInfoUpdatedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        for (i32 x : v_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        for (i32& x : v_) x = stream().readVInt();
    }
    LogicLong mapId_;
    i32 v_[5] = {};
};

} // namespace titan
