#pragma once

// ListBrawlTvChannelsMessage (14700) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ListBrawlTvChannelsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14700; }
    const char* getMessageTypeName() const override { return "ListBrawlTvChannelsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
        flag_ = stream().readBoolean();
    }
    i32 v_ = 0;
    bool flag_ = false;
};

} // namespace titan
