#pragma once

// EventData::encode @0x247168, decode @0x237294.
// 25-field daily-event descriptor; nested BattlePlayerMap/ChronosTextEntry/
// LogicGemOffer/LogicRankedSeason/ChronosFileEntry stay pending.
// Split out of the entry wave; wire format unchanged.

#include "titan/core/DataReference.cpp"
#include "titan/messages/Nested.cpp"
#include "titan/messages/pending/BattlePlayerMap.cpp"
#include "titan/game/home/ChronosFileEntry.cpp"
#include "titan/game/home/ChronosTextEntry.cpp"
#include "titan/game/shop/LogicGemOffer.cpp"
#include "titan/game/player/LogicRankedSeason.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class EventData : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    i32 v_[6] = {}; // +0..+20
    std::optional<DataReference> ref_; // +24
    i32 w32_ = 0, w36_ = 0; // +32/+36
    std::optional<std::string> name_; // +40
    i32 w48_ = 0, w52_ = 0, w56_ = 0; // +48/+52/+56
    std::vector<i32> vintLoop_; // +64 count + loop
    i32 w80_ = 0, w92_ = 0; // +80/+92
    std::unique_ptr<BattlePlayerMap> map_; // +120
    i32 w60_ = 0; // +60
    std::unique_ptr<LogicRankedSeason> season_; // +128 optional
    i32 w84_ = 0, w88_ = 0; // +84/+88
    std::unique_ptr<ChronosTextEntry> chrono1_, chrono2_; // +96/+104 optional
    std::unique_ptr<LogicGemOffer> offer_; // +136 optional
    std::vector<i32> optLoop_; // +72 vint-count (-1=null) + loop
    bool optLoopNull_ = false;
    std::unique_ptr<ChronosFileEntry> file_; // +112 optional
};

} // namespace titan

// EventData bodies (see EventData.hpp).


namespace titan {

template <typename T>
static void encodeOpt(ByteStream& s, const std::unique_ptr<T>& ptr, const char* ctx) {
    s.writeBoolean(static_cast<bool>(ptr));
    if (ptr) {
        if (!ptr) throw pending_reverse(ctx);
        ptr->encode(s);
    }
}

inline void EventData::encode(ByteStream& s) const {
    for (i32 x : v_) s.writeVInt(x);
    DataReference::encodeNullable(s, ref_);
    s.writeVInt(w32_);
    s.writeVInt(w36_);
    s.writeString(name_ ? &*name_ : nullptr);
    s.writeVInt(w48_);
    s.writeVInt(w52_);
    s.writeVInt(w56_);
    s.writeVInt(static_cast<i32>(vintLoop_.size()));
    for (i32 x : vintLoop_) s.writeVInt(x);
    s.writeVInt(w80_);
    s.writeVInt(w92_);
    if (!map_) throw pending_reverse("EventData needs BattlePlayerMap");
    map_->encode(s);
    s.writeVInt(w60_);
    encodeOpt(s, season_, "EventData needs LogicRankedSeason");
    s.writeVInt(w84_);
    s.writeVInt(w88_);
    encodeOpt(s, chrono1_, "EventData needs ChronosTextEntry");
    encodeOpt(s, chrono2_, "EventData needs ChronosTextEntry");
    encodeOpt(s, offer_, "EventData needs LogicGemOffer");
    if (optLoopNull_) {
        s.writeVInt(-1);
    } else {
        s.writeVInt(static_cast<i32>(optLoop_.size()));
        for (i32 x : optLoop_) s.writeVInt(x);
    }
    encodeOpt(s, file_, "EventData needs ChronosFileEntry");
}

template <typename T>
static void decodeOpt(ByteStream& s, std::unique_ptr<T>& ptr) {
    if (s.readBoolean()) {
        ptr = std::make_unique<T>();
        ptr->decode(s);
    }
}

inline void EventData::decode(ByteStream& s) {
    for (i32& x : v_) x = s.readVInt();
    ref_ = DataReference::decodeNullable(s);
    w32_ = s.readVInt();
    w36_ = s.readVInt();
    name_ = s.readString();
    w48_ = s.readVInt();
    w52_ = s.readVInt();
    w56_ = s.readVInt();
    const i32 n = s.readVInt();
    vintLoop_.clear();
    for (i32 i = 0; i < n; ++i) vintLoop_.push_back(s.readVInt());
    w80_ = s.readVInt();
    w92_ = s.readVInt();
    map_ = std::make_unique<BattlePlayerMap>();
    map_->decode(s);
    w60_ = s.readVInt();
    decodeOpt(s, season_);
    w84_ = s.readVInt();
    w88_ = s.readVInt();
    decodeOpt(s, chrono1_);
    decodeOpt(s, chrono2_);
    decodeOpt(s, offer_);
    const i32 m = s.readVInt();
    optLoop_.clear();
    optLoopNull_ = (m < 0);
    for (i32 i = 0; i < m; ++i) optLoop_.push_back(s.readVInt());
    decodeOpt(s, file_);
}

} // namespace titan
