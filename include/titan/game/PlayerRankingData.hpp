#pragma once

// PlayerRankingData::encode @0x8a6d9c (const), decode @0x74c4a8.
// Wire: string (nullable String* @+8), PlayerDisplayData @+16 (required).
// Decode mirrors the same order.

#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <string>

namespace titan {

class PlayerRankingData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeString(name_.has_value() ? &name_.value() : nullptr);
        if (!display_) throw pending_reverse("PlayerRankingData needs PlayerDisplayData");
        display_->encode(s);
    }
    void decode(ByteStream& s) override {
        name_ = s.readString();
        display_ = std::make_unique<PlayerDisplayData>();
        display_->decode(s);
    }

    std::optional<std::string> name_; // +8
    std::unique_ptr<PlayerDisplayData> display_; // +16 (required)
};

} // namespace titan
