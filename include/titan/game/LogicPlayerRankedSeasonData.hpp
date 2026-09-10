#pragma once

// LogicPlayerRankedSeasonData::encode @0x286950, decode @0x97acd8.
// Wire: 11 × vint (+8..+48), vint count + LogicPlayerRewardData[]
// (LogicArrayList @+56, count +68). vtable @+0.

#include "titan/game/LogicPlayerRewardData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <vector>

namespace titan {

class LogicPlayerRankedSeasonData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : head_) s.writeVInt(v);
        s.writeVInt(static_cast<i32>(rewards_.size()));
        for (const auto& r : rewards_) r->encode(s);
    }
    void decode(ByteStream& s) override {
        for (i32& v : head_) v = s.readVInt();
        const i32 n = s.readVInt();
        rewards_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto r = std::make_unique<LogicPlayerRewardData>();
            r->decode(s);
            rewards_.push_back(std::move(r));
        }
    }

    i32 head_[11] = {}; // +8..+48
    std::vector<std::unique_ptr<LogicPlayerRewardData>> rewards_; // +56 (+68)
};

} // namespace titan
