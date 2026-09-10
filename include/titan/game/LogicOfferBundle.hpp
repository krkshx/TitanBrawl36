#pragma once

// LogicOfferBundle::encode @0x69b644 (size 0x68).
// Wire order (offsets are binary object offsets):
//   vint gemCount + LogicGemOffer[] (array @+8, count +20),
//   vint +28, vint +24, vint +32, vint +36, vint +40, bool +44,
//   vint +48, vint +52, bool +56, vint +80,
//   ChronosTextEntry (@+64, required), bool +88,
//   string (@+72, nullable), vint +84, bool +100, vint +92, vint +96.
// Decode is the stream ctor used by LogicDailyData::decode: same reads
// in the same order (mirrored below).

#include "titan/game/ChronosTextEntry.hpp"
#include "titan/game/LogicGemOffer.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicOfferBundle : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(gems_.size()));
        for (const auto& g : gems_) g->encode(s);
        s.writeVInt(v28_);
        s.writeVInt(v24_);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        s.writeVInt(v40_);
        s.writeBoolean(b44_);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
        s.writeBoolean(b56_);
        s.writeVInt(v80_);
        if (!text_) throw pending_reverse("LogicOfferBundle needs ChronosTextEntry");
        text_->encode(s);
        s.writeBoolean(b88_);
        s.writeString(str_.has_value() ? &str_.value() : nullptr);
        s.writeVInt(v84_);
        s.writeBoolean(b100_);
        s.writeVInt(v92_);
        s.writeVInt(v96_);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        gems_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto g = std::make_unique<LogicGemOffer>();
            g->decode(s);
            gems_.push_back(std::move(g));
        }
        v28_ = s.readVInt();
        v24_ = s.readVInt();
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        v40_ = s.readVInt();
        b44_ = s.readBoolean();
        v48_ = s.readVInt();
        v52_ = s.readVInt();
        b56_ = s.readBoolean();
        v80_ = s.readVInt();
        text_ = std::make_unique<ChronosTextEntry>();
        text_->decode(s);
        b88_ = s.readBoolean();
        str_ = s.readString();
        v84_ = s.readVInt();
        b100_ = s.readBoolean();
        v92_ = s.readVInt();
        v96_ = s.readVInt();
    }

    std::vector<std::unique_ptr<LogicGemOffer>> gems_; // +8 (+20 count)
    i32 v24_ = 0;                                     // +24
    i32 v28_ = 0;                                     // +28
    i32 v32_ = 0, v36_ = 0, v40_ = 0;                 // +32..+40
    bool b44_ = false;                                // +44
    i32 v48_ = 0, v52_ = 0;                           // +48, +52
    bool b56_ = false;                                // +56
    std::unique_ptr<ChronosTextEntry> text_;          // +64 (required)
    std::optional<std::string> str_;                  // +72 String
    i32 v80_ = 0;                                     // +80
    i32 v84_ = 0;                                     // +84
    bool b88_ = false;                                // +88
    i32 v92_ = 0, v96_ = 0;                           // +92, +96
    bool b100_ = false;                               // +100
};

} // namespace titan
