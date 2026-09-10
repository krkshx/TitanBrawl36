#pragma once

// PlayerMapGreenlightedMessage (22111) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PlayerMapGreenlightedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22111; }
    const char* getMessageTypeName() const override { return "PlayerMapGreenlightedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        stream().writeVInt(f140_);
        stream().writeVInt(f144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        f140_ = stream().readVInt();
        f144_ = stream().readVInt();
    }
    LogicLong mapId_;
    i32 f140_ = 0, f144_ = 0;
};

} // namespace titan
