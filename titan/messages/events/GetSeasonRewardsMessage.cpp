#pragma once

// GetSeasonRewardsMessage (14277) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GetSeasonRewardsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14277; }
    const char* getMessageTypeName() const override { return "GetSeasonRewardsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(season_);
    }
    void decode() override {
        PiranhaMessage::decode();
        season_ = stream().readVInt();
    }
    i32 season_ = 0;
};

} // namespace titan
