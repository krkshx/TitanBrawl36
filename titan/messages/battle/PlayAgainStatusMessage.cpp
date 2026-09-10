#pragma once

// PlayAgainStatusMessage (24777) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/PlayAgainStatus.hpp"

namespace titan {

class PlayAgainStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24777; }
    const char* getMessageTypeName() const override { return "PlayAgainStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!status_) throw pending_reverse("PlayAgainStatusMessage needs PlayAgainStatus");
        status_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        status_ = std::make_unique<PlayAgainStatus>();
        status_->decode(stream());
    }
    std::unique_ptr<PlayAgainStatus> status_;
};

} // namespace titan
