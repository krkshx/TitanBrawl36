#pragma once

// LogicConfData::encode @0x653b94, decode @0x6f6508.
// Order below follows the binary exactly (counts + arrays):
//   vint +0,
//   vint count + EventSlot[] (@+8, count +20),
//   vint count + EventData[] (@+24, count +36),
//   vint count + EventData[] (@+40, count +52),
//   vint count + int[] (@+56, count +68),
//   vint count + int[] (@+72, count +84),
//   vint count + int[] (@+88, count +100),
//   bool +104,
//   vint count + ReleaseEntry[] (@+112, count +124),
//   vint count + IntValueEntry[] (@+128, count +140),
//   vint count + TimedIntValueEntry[] (@+144, count +156),
//   vint count + CustomEvent[] (@+160, count +172).

#include "titan/game/util/CustomEvent.cpp"
#include "titan/game/home/EventData.cpp"
#include "titan/game/home/EventSlot.cpp"
#include "titan/game/player/IntValueEntry.cpp"
#include "titan/game/util/ReleaseEntry.cpp"
#include "titan/game/player/TimedIntValueEntry.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <vector>

namespace titan {

class LogicConfData : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;

    // getLevelUpCost @0x708b44: ints17_[level-1] when level <= size, else 0.
    [[nodiscard]] i32 getLevelUpCost(int level) const {
        if (level < 1 || level > static_cast<int>(ints17_.size())) return 0;
        return ints17_[static_cast<std::size_t>(level - 1)];
    }
    // getIntValueEntry @0x4698d8: first intValues_ entry with a_ == id.
    // getIntValue @0x51a1f0: entry value or getDefaultIntValue(id).
    // getIntValue(id, fallback) @0x26187c: entry value or caller default.
    // getDefaultIntValue @0x999c84: hardcoded table below.
    [[nodiscard]] const IntValueEntry* getIntValueEntry(i32 id) const;
    [[nodiscard]] i32 getIntValue(i32 id) const;
    [[nodiscard]] i32 getIntValue(i32 id, i32 fallback) const;
    [[nodiscard]] static i32 getDefaultIntValue(i32 id);

    i32 v0_ = 0; // +0
    std::vector<std::unique_ptr<EventSlot>> slots_;          // +8 (+20)
    std::vector<std::unique_ptr<EventData>> events1_;        // +24 (+36)
    std::vector<std::unique_ptr<EventData>> events2_;        // +40 (+52)
    std::vector<i32> ints17_;                                // +56 (+68)
    std::vector<i32> ints21_;                                // +72 (+84)
    std::vector<i32> ints25_;                                // +88 (+100)
    bool b104_ = false;                                      // +104
    std::vector<std::unique_ptr<ReleaseEntry>> releases_;    // +112 (+124)
    std::vector<std::unique_ptr<IntValueEntry>> intValues_;  // +128 (+140)
    std::vector<std::unique_ptr<TimedIntValueEntry>> timed_; // +144 (+156)
    std::vector<std::unique_ptr<CustomEvent>> customs_;      // +160 (+172)
};

} // namespace titan

// LogicConfData bodies — encode @0x653b94, decode @0x6f6508.
// Order of reads/writes mirrors the binary exactly (see header).


namespace titan {

inline void LogicConfData::encode(ByteStream& s) const {
    s.writeVInt(v0_);
    s.writeVInt(static_cast<i32>(slots_.size()));
    for (const auto& e : slots_) e->encode(s);
    s.writeVInt(static_cast<i32>(events1_.size()));
    for (const auto& e : events1_) e->encode(s);
    s.writeVInt(static_cast<i32>(events2_.size()));
    for (const auto& e : events2_) e->encode(s);
    s.writeVInt(static_cast<i32>(ints17_.size()));
    for (const i32 v : ints17_) s.writeVInt(v);
    s.writeVInt(static_cast<i32>(ints21_.size()));
    for (const i32 v : ints21_) s.writeVInt(v);
    s.writeVInt(static_cast<i32>(ints25_.size()));
    for (const i32 v : ints25_) s.writeVInt(v);
    s.writeBoolean(b104_);
    s.writeVInt(static_cast<i32>(releases_.size()));
    for (const auto& e : releases_) e->encode(s);
    s.writeVInt(static_cast<i32>(intValues_.size()));
    for (const auto& e : intValues_) e->encode(s);
    s.writeVInt(static_cast<i32>(timed_.size()));
    for (const auto& e : timed_) e->encode(s);
    s.writeVInt(static_cast<i32>(customs_.size()));
    for (const auto& e : customs_) e->encode(s);
}

inline void LogicConfData::decode(ByteStream& s) {
    v0_ = s.readVInt();
    const i32 nSlots = s.readVInt();
    slots_.clear();
    for (i32 i = 0; i < nSlots; ++i) {
        auto e = std::make_unique<EventSlot>();
        e->decode(s);
        slots_.push_back(std::move(e));
    }
    const i32 nE1 = s.readVInt();
    events1_.clear();
    for (i32 i = 0; i < nE1; ++i) {
        auto e = std::make_unique<EventData>();
        e->decode(s);
        events1_.push_back(std::move(e));
    }
    const i32 nE2 = s.readVInt();
    events2_.clear();
    for (i32 i = 0; i < nE2; ++i) {
        auto e = std::make_unique<EventData>();
        e->decode(s);
        events2_.push_back(std::move(e));
    }
    const i32 n17 = s.readVInt();
    ints17_.clear();
    for (i32 i = 0; i < n17; ++i) ints17_.push_back(s.readVInt());
    const i32 n21 = s.readVInt();
    ints21_.clear();
    for (i32 i = 0; i < n21; ++i) ints21_.push_back(s.readVInt());
    const i32 n25 = s.readVInt();
    ints25_.clear();
    for (i32 i = 0; i < n25; ++i) ints25_.push_back(s.readVInt());
    b104_ = s.readBoolean();
    const i32 nRel = s.readVInt();
    releases_.clear();
    for (i32 i = 0; i < nRel; ++i) {
        auto e = std::make_unique<ReleaseEntry>();
        e->decode(s);
        releases_.push_back(std::move(e));
    }
    const i32 nIv = s.readVInt();
    intValues_.clear();
    for (i32 i = 0; i < nIv; ++i) {
        auto e = std::make_unique<IntValueEntry>();
        e->decode(s);
        intValues_.push_back(std::move(e));
    }
    const i32 nT = s.readVInt();
    timed_.clear();
    for (i32 i = 0; i < nT; ++i) {
        auto e = std::make_unique<TimedIntValueEntry>();
        e->decode(s);
        timed_.push_back(std::move(e));
    }
    const i32 nC = s.readVInt();
    customs_.clear();
    for (i32 i = 0; i < nC; ++i) {
        auto e = std::make_unique<CustomEvent>();
        e->decode(s);
        customs_.push_back(std::move(e));
    }
}

inline const IntValueEntry* LogicConfData::getIntValueEntry(i32 id) const {
    for (const auto& e : intValues_) {
        if (e && e->a_ == id) return e.get();
    }
    return nullptr;
}

inline i32 LogicConfData::getIntValue(i32 id) const {
    const IntValueEntry* e = getIntValueEntry(id);
    return e ? e->b_ : getDefaultIntValue(id);
}

inline i32 LogicConfData::getIntValue(i32 id, i32 fallback) const {
    const IntValueEntry* e = getIntValueEntry(id);
    return e ? e->b_ : fallback;
}

inline i32 LogicConfData::getDefaultIntValue(i32 id) {
    switch (id - 10000) {
        case 0: return 100;
        case 1: case 5: case 16: case 32: case 33: return 10;
        case 2: return 30;
        case 3: return 3;
        case 4: return 80;
        case 6: return 40;
        case 7: case 27: case 28: case 35: case 36: case 37: return 1000;
        case 8: return 550;
        case 10: return 999900;
        case 12: case 39: return 2;
        case 13: return 200;
        case 14: return 20;
        case 15: return 8640;
        case 17: case 29: return 5;
        case 19: case 20: case 23: case 24: case 25: case 26:
        case 30: case 31: case 41: case 42: return 1;
        case 21: case 34: return 50;
        case 22: return 604800;
        case 43: return 300;
        default: return 0;
    }
}

} // namespace titan
