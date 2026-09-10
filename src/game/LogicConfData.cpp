// LogicConfData bodies — encode @0x653b94, decode @0x6f6508.
// Order of reads/writes mirrors the binary exactly (see header).

#include "titan/game/LogicConfData.hpp"

namespace titan {

void LogicConfData::encode(ByteStream& s) const {
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

void LogicConfData::decode(ByteStream& s) {
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

} // namespace titan
