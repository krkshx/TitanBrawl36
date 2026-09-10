#pragma once

// LogicRankedMatch::encode @0x400e0c (const), decode @0x47c4c8.
// Wire: LogicUuid @+8 (embedded), dataref (nullable @+32), bool @+40,
//   nullable LogicRankedMatchPlayer[] @+48 (null -> vint -1, else count +
//   entries), vint @+56.
// Decode mirrors the same order.

#include "titan/core/DataReference.hpp"
#include "titan/game/LogicRankedMatchPlayer.hpp"
#include "titan/game/LogicUuid.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <vector>

namespace titan {

class LogicRankedMatch : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        uuid_.encode(s);
        DataReference::encodeNullable(s, ref32_);
        s.writeBoolean(b40_);
        if (!players_) {
            s.writeVInt(-1);
        } else {
            s.writeVInt(static_cast<i32>(players_->size()));
            for (const auto& p : *players_) p->encode(s);
        }
        s.writeVInt(v56_);
    }
    void decode(ByteStream& s) override {
        uuid_.decode(s);
        ref32_ = DataReference::decodeNullable(s);
        b40_ = s.readBoolean();
        const i32 n = s.readVInt();
        if (n < 0) {
            players_.reset();
        } else {
            players_.emplace();
            for (i32 i = 0; i < n; ++i) {
                auto p = std::make_unique<LogicRankedMatchPlayer>();
                p->decode(s);
                players_->push_back(std::move(p));
            }
        }
        v56_ = s.readVInt();
    }

    LogicUuid uuid_; // +8 (embedded)
    std::optional<DataReference> ref32_; // +32
    bool b40_ = false;
    std::optional<std::vector<std::unique_ptr<LogicRankedMatchPlayer>>> players_; // +48
    i32 v56_ = 0;
};

} // namespace titan
