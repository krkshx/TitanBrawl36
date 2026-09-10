#pragma once

// LogicRankedMatchRoundState::encode @0x8e26bc (const).
// Wire: 11 × vint @+8..+48,
//   nullable LogicPlayerRewardData[] @+56 (null -> vint -1),
//   nullable LogicPlayerRewardData[] @+64 (null -> vint -1),
//   bool + LogicRankRewardConfig @+72 (nullable),
//   bool + LogicRankedMatchResultDebugInfo @+80 (nullable).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/game/LogicPlayerRewardData.hpp"
#include "titan/game/LogicRankRewardConfig.hpp"
#include "titan/game/LogicRankedMatchResultDebugInfo.hpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <optional>
#include <vector>

namespace titan {

class LogicRankedMatchRoundState : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : head_) s.writeVInt(v);
        encodeRewardList(s, rewards1_);
        encodeRewardList(s, rewards2_);
        s.writeBoolean(rankReward_ != nullptr);
        if (rankReward_) rankReward_->encode(s);
        s.writeBoolean(debug_ != nullptr);
        if (debug_) debug_->encode(s);
    }
    void decode(ByteStream& s) override {
        for (i32& v : head_) v = s.readVInt();
        decodeRewardList(s, rewards1_);
        decodeRewardList(s, rewards2_);
        if (s.readBoolean()) {
            rankReward_ = std::make_unique<LogicRankRewardConfig>();
            rankReward_->decode(s);
        } else {
            rankReward_.reset();
        }
        if (s.readBoolean()) {
            debug_ = std::make_unique<LogicRankedMatchResultDebugInfo>();
            debug_->decode(s);
        } else {
            debug_.reset();
        }
    }

    i32 head_[11] = {}; // +8..+48
    std::optional<std::vector<std::unique_ptr<LogicPlayerRewardData>>> rewards1_; // +56
    std::optional<std::vector<std::unique_ptr<LogicPlayerRewardData>>> rewards2_; // +64
    std::unique_ptr<LogicRankRewardConfig> rankReward_; // +72 (nullable)
    std::unique_ptr<LogicRankedMatchResultDebugInfo> debug_; // +80 (nullable)

private:
    using RewardList = std::optional<std::vector<std::unique_ptr<LogicPlayerRewardData>>>;
    static void encodeRewardList(ByteStream& s, const RewardList& list) {
        if (!list) {
            s.writeVInt(-1);
            return;
        }
        s.writeVInt(static_cast<i32>(list->size()));
        for (const auto& r : *list) r->encode(s);
    }
    static void decodeRewardList(ByteStream& s, RewardList& list) {
        const i32 n = s.readVInt();
        if (n < 0) {
            list.reset();
            return;
        }
        list.emplace();
        for (i32 i = 0; i < n; ++i) {
            auto r = std::make_unique<LogicPlayerRewardData>();
            r->decode(s);
            list->push_back(std::move(r));
        }
    }
};

} // namespace titan
