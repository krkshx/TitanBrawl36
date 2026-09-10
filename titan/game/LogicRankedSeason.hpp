#pragma once

// LogicRankedSeason::encode @0x2a66b0 (const).
// Wire: vint @+8, string (nullable String* @+16), vint @+24/+28,
//   vint count + LogicRewardConfig[] (@+32, count +44),
//   vint count + LogicRankRewardConfig[] (@+48, count +60).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/game/LogicRankRewardConfig.hpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/LogicRewardConfig.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicRankedSeason : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v8_);
        s.writeString(name_.has_value() ? &name_.value() : nullptr);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
        s.writeVInt(static_cast<i32>(rewards_.size()));
        for (const auto& r : rewards_) r->encode(s);
        s.writeVInt(static_cast<i32>(rankRewards_.size()));
        for (const auto& r : rankRewards_) r->encode(s);
    }
    void decode(ByteStream& s) override {
        v8_ = s.readVInt();
        name_ = s.readString();
        v24_ = s.readVInt();
        v28_ = s.readVInt();
        const i32 n = s.readVInt();
        rewards_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto r = std::make_unique<LogicRewardConfig>();
            r->decode(s);
            rewards_.push_back(std::move(r));
        }
        const i32 m = s.readVInt();
        rankRewards_.clear();
        for (i32 i = 0; i < m; ++i) {
            auto r = std::make_unique<LogicRankRewardConfig>();
            r->decode(s);
            rankRewards_.push_back(std::move(r));
        }
    }

    i32 v8_ = 0;
    std::optional<std::string> name_; // +16
    i32 v24_ = 0, v28_ = 0;
    std::vector<std::unique_ptr<LogicRewardConfig>> rewards_; // +32 (+44)
    std::vector<std::unique_ptr<LogicRankRewardConfig>> rankRewards_; // +48 (+60)
};

} // namespace titan
