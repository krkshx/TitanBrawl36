#pragma once

// GetLeaderboardMessage (14403) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GetLeaderboardMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14403; }
    const char* getMessageTypeName() const override { return "GetLeaderboardMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(flag_);
        stream().writeVInt(leaderboardType_);
        DataReference::encodeNullable(stream(), leaderboard_);
        stream().writeVInt(unknown_);
    }
    void decode() override {
        PiranhaMessage::decode();
        flag_ = stream().readBoolean();
        leaderboardType_ = stream().readVInt();
        leaderboard_ = DataReference::decodeNullable(stream());
        unknown_ = stream().readVInt();
    }
    bool flag_ = false;
    i32 leaderboardType_ = 0, unknown_ = 0;
    std::optional<DataReference> leaderboard_;
};

} // namespace titan
