// EventData bodies (see EventData.hpp).

#include "titan/game/EventData.hpp"

namespace titan {

template <typename T>
static void encodeOpt(ByteStream& s, const std::unique_ptr<T>& ptr, const char* ctx) {
    s.writeBoolean(static_cast<bool>(ptr));
    if (ptr) {
        if (!ptr) throw pending_reverse(ctx);
        ptr->encode(s);
    }
}

void EventData::encode(ByteStream& s) const {
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

void EventData::decode(ByteStream& s) {
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
