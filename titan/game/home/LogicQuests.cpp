#pragma once

// LogicQuests::encode @0x1ae36c.
// Wire: vint count + QuestData[] (LogicArrayList @+0, count +12).
// Decode is the stream ctor used by LogicDailyData::decode: same reads
// in the same order (mirrored below). Non-empty lists throw
// pending_reverse until QuestData lands.

#include "titan/messages/Nested.cpp"
#include "titan/game/home/QuestData.cpp"

#include <memory>
#include <vector>

namespace titan {

class LogicQuests : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(quests_.size()));
        for (const auto& q : quests_) q->encode(s);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        quests_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto q = std::make_unique<QuestData>();
            q->decode(s);
            quests_.push_back(std::move(q));
        }
    }

    std::vector<std::unique_ptr<QuestData>> quests_;
};

} // namespace titan
