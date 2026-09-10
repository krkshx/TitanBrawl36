#pragma once

// LogicRewards::encode @0x2c2fac.
// Wire: vint count + LogicGemOffer[] (LogicArrayList @+8, count +20).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/game/LogicGemOffer.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <vector>

namespace titan {

class LogicRewards : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(offers_.size()));
        for (const auto& o : offers_) o->encode(s);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        offers_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto o = std::make_unique<LogicGemOffer>();
            o->decode(s);
            offers_.push_back(std::move(o));
        }
    }

    std::vector<std::unique_ptr<LogicGemOffer>> offers_; // +8 (+20)
};

} // namespace titan
