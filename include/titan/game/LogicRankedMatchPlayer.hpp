#pragma once

// LogicRankedMatchPlayer::encode @0x25999c (const).
// Wire: encodeLogicLong @+8 (embedded LogicLong, vint pair),
//   bool + PlayerDisplayData @+16 (nullable), vint +24/+28/+32,
//   dataref (nullable @+40), vint +48, dataref @+56/+64/+72 (nullable),
//   bool @+80.
// Decode mirrors the same order (stream ctor used by LogicRankedMatch::decode).

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>

namespace titan {

class LogicRankedMatchPlayer : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        encodeLogicLong(s, id8_);
        s.writeBoolean(display_ != nullptr);
        if (display_) display_->encode(s);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
        s.writeVInt(v32_);
        DataReference::encodeNullable(s, ref40_);
        s.writeVInt(v48_);
        DataReference::encodeNullable(s, ref56_);
        DataReference::encodeNullable(s, ref64_);
        DataReference::encodeNullable(s, ref72_);
        s.writeBoolean(b80_);
    }
    void decode(ByteStream& s) override {
        id8_.high = s.readVInt();
        id8_.low = s.readVInt();
        if (s.readBoolean()) {
            display_ = std::make_unique<PlayerDisplayData>();
            display_->decode(s);
        } else {
            display_.reset();
        }
        v24_ = s.readVInt();
        v28_ = s.readVInt();
        v32_ = s.readVInt();
        ref40_ = DataReference::decodeNullable(s);
        v48_ = s.readVInt();
        ref56_ = DataReference::decodeNullable(s);
        ref64_ = DataReference::decodeNullable(s);
        ref72_ = DataReference::decodeNullable(s);
        b80_ = s.readBoolean();
    }

    LogicLong id8_; // +8 (vint pair)
    std::unique_ptr<PlayerDisplayData> display_; // +16 (nullable)
    i32 v24_ = 0, v28_ = 0, v32_ = 0;
    std::optional<DataReference> ref40_;
    i32 v48_ = 0;
    std::optional<DataReference> ref56_, ref64_, ref72_;
    bool b80_ = false;
};

} // namespace titan
