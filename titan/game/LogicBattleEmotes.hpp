#pragma once

// LogicBattleEmotes::encode @0x759184.
// Wire: vint count + dataref[] (LogicArrayList @+0, count +12).
// Decode is default-ctor + decode (see LogicPlayer::decode @0x89bacc):
// same reads in the same order (mirrored below).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.cpp"

#include <vector>

namespace titan {

class LogicBattleEmotes : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(emotes_.size()));
        for (const auto& r : emotes_) r.encode(s);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        emotes_.clear();
        for (i32 i = 0; i < n; ++i) {
            DataReference r;
            r.classId = s.readVInt();
            r.instanceId = s.readVInt();
            emotes_.push_back(r);
        }
    }

    std::vector<DataReference> emotes_;
};

} // namespace titan
