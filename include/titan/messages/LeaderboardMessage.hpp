#pragma once

// LeaderboardMessage (24403) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/RankingEntry.hpp"

namespace titan {

class LeaderboardMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24403; }
    const char* getMessageTypeName() const override { return "LeaderboardMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0, v136_ = 0;
    std::optional<DataReference> ref_;
    std::optional<std::string> name_;
    std::vector<std::unique_ptr<RankingEntry>> rankings_;
    i32 tail_[4] = {}; // v168..v180
    std::optional<std::string> tailStr_;
};

} // namespace titan
