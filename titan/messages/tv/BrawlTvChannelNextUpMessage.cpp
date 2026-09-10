#pragma once

// BrawlTvChannelNextUpMessage (24701) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class BrawlTvChannelNextUpMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24701; }
    const char* getMessageTypeName() const override { return "BrawlTvChannelNextUpMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
        stream().writeBoolean(hasBattle_);
        if (hasBattle_) battleId_.encode(stream());
        else stream().writeStringReference(stringRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
        hasBattle_ = stream().readBoolean();
        if (hasBattle_) battleId_ = LogicLong::decode(stream());
        else stringRef_ = stream().readStringReference();
    }
    i32 v132_ = 0, v136_ = 0;
    bool hasBattle_ = false;
    LogicLong battleId_;
    std::string stringRef_;
};

} // namespace titan
