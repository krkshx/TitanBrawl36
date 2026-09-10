#pragma once

// TuneBrawlTvChannelMessage (14701) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TuneBrawlTvChannelMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14701; }
    const char* getMessageTypeName() const override { return "TuneBrawlTvChannelMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

} // namespace titan
