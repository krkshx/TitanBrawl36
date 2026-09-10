#pragma once

// BrawlPassSeasonData::encode @0x5e8140.
// Wire: vint @+0, vint @+4, bool @+24, vint @+28, bool @+32,
//   bool + LogicBitList @+8 (nullable), bool + LogicBitList @+16 (nullable).
// Decode is the stream ctor used by LogicDailyData::decode: same reads
// in the same order (mirrored below).

#include "titan/messages/Nested.hpp"
#include "titan/messages/pending/LogicBitList.hpp"

#include <memory>

namespace titan {

class BrawlPassSeasonData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v1_);
        s.writeBoolean(b24_);
        s.writeVInt(v28_);
        s.writeBoolean(b32_);
        s.writeBoolean(bits1_ != nullptr);
        if (bits1_) bits1_->encode(s);
        s.writeBoolean(bits2_ != nullptr);
        if (bits2_) bits2_->encode(s);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v1_ = s.readVInt();
        b24_ = s.readBoolean();
        v28_ = s.readVInt();
        b32_ = s.readBoolean();
        if (s.readBoolean()) {
            bits1_ = std::make_unique<LogicBitList>();
            bits1_->decode(s);
        } else {
            bits1_.reset();
        }
        if (s.readBoolean()) {
            bits2_ = std::make_unique<LogicBitList>();
            bits2_->decode(s);
        } else {
            bits2_.reset();
        }
    }

    i32 v0_ = 0;
    i32 v1_ = 0;
    bool b24_ = false;
    i32 v28_ = 0;
    bool b32_ = false;
    std::unique_ptr<LogicBitList> bits1_; // +8 (nullable)
    std::unique_ptr<LogicBitList> bits2_; // +16 (nullable)
};

} // namespace titan
