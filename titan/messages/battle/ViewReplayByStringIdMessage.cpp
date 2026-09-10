#pragma once

// ViewReplayByStringIdMessage (14116) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ViewReplayByStringIdMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14116; }
    const char* getMessageTypeName() const override { return "ViewReplayByStringIdMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(replayId_);
    }
    void decode() override {
        PiranhaMessage::decode();
        replayId_ = stream().readStringReference();
    }
    std::string replayId_;
};

} // namespace titan
