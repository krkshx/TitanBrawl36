#pragma once

// PlayerProfile::encode @0x693e7c, decode @0x619598.
// Wire: logiclong, dataref, vint count + HeroEntry loop,
// vint count + (vint, vint) pairs, PlayerDisplayData.
// HeroEntry stays pending. Split out of the entry wave.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Nested.hpp"
#include "titan/game/HeroEntry.hpp"

#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace titan {

class PlayerProfile : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(static_cast<i32>(heroes_.size()));
        for (const auto& h : heroes_) h->encode(s);
        s.writeVInt(static_cast<i32>(pairs_.size()));
        for (const auto& [a, b] : pairs_) {
            s.writeVInt(a);
            s.writeVInt(b);
        }
        display_.encode(s);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        ref_ = DataReference::decodeNullable(s);
        const i32 n = s.readVInt();
        heroes_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto h = std::make_unique<HeroEntry>();
            h->decode(s);
            heroes_.push_back(std::move(h));
        }
        const i32 m = s.readVInt();
        pairs_.clear();
        for (i32 i = 0; i < m; ++i) {
            const i32 a = s.readVInt();
            const i32 b = s.readVInt();
            pairs_.emplace_back(a, b);
        }
        display_.decode(s);
    }
    LogicLong id_;
    std::optional<DataReference> ref_;
    std::vector<std::unique_ptr<HeroEntry>> heroes_;
    std::vector<std::pair<i32, i32>> pairs_;
    PlayerDisplayData display_;
};

} // namespace titan
