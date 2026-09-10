#pragma once

// TeamMessage (24124) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/TeamEntry.hpp"

namespace titan {

class TeamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24124; }
    const char* getMessageTypeName() const override { return "TeamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!team_) throw pending_reverse("TeamMessage needs TeamEntry");
        team_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        team_ = std::make_unique<TeamEntry>();
        team_->decode(stream());
    }
    std::unique_ptr<TeamEntry> team_;
};

} // namespace titan
