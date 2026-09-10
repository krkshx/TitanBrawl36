#pragma once

// BattleLogEntry::encode @0x8e4504 (decode via stream-ctor @0x2e6380).
// Wire: vint x5 (+0/+36/+4/+8/+12), bool +16, dataref +24, vint +32/+40,
// int64 +48/+56, vint +64, bool +68, vint count + BattleLogPlayerEntry loop,
// vint +72, bool +76, vint +80, bool +84, vint +88,
// optional +112 (bool + logiclong + vint + stringref + logiclong),
// optional +152 (bool + LogicUuid + bool + vint + vint; absent -> final vint 0).
// BattleLogPlayerEntry/LogicUuid stay pending. Split out of the entry wave.

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/battle/BattleLogPlayerEntry.cpp"
#include "titan/game/util/LogicUuid.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class BattleLogEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    i32 v0_ = 0, v36_ = 0, v4_ = 0, v8_ = 0, v12_ = 0;
    bool b16_ = false;
    std::optional<DataReference> ref_;
    i32 v32_ = 0, v40_ = 0;
    i64 q48_ = 0, q56_ = 0;
    i32 v64_ = 0;
    bool b68_ = false;
    std::vector<std::unique_ptr<BattleLogPlayerEntry>> players_;
    i32 v72_ = 0;
    bool b76_ = false;
    i32 v80_ = 0;
    bool b84_ = false;
    i32 v88_ = 0;
    bool hasOpt112_ = false;
    LogicLong id112_, id144_;
    i32 v120_ = 0;
    std::string ref128_;
    bool hasOpt152_ = false;
    std::unique_ptr<LogicUuid> uuid_;
    bool b160_ = false;
    i32 v164_ = 0, v168_ = 0;
};

} // namespace titan

// BattleLogEntry bodies (see BattleLogEntry.hpp).


namespace titan {

inline void BattleLogEntry::encode(ByteStream& s) const {
    s.writeVInt(v0_);
    s.writeVInt(v36_);
    s.writeVInt(v4_);
    s.writeVInt(v8_);
    s.writeVInt(v12_);
    s.writeBoolean(b16_);
    DataReference::encodeNullable(s, ref_);
    s.writeVInt(v32_);
    s.writeVInt(v40_);
    s.writeLongLong(q48_);
    s.writeLongLong(q56_);
    s.writeVInt(v64_);
    s.writeBoolean(b68_);
    s.writeVInt(static_cast<i32>(players_.size()));
    for (const auto& p : players_) p->encode(s);
    s.writeVInt(v72_);
    s.writeBoolean(b76_);
    s.writeVInt(v80_);
    s.writeBoolean(b84_);
    s.writeVInt(v88_);
    s.writeBoolean(hasOpt112_);
    if (hasOpt112_) {
        id112_.encode(s);
        s.writeVInt(v120_);
        s.writeStringReference(ref128_);
        id144_.encode(s);
    }
    s.writeBoolean(hasOpt152_);
    if (hasOpt152_) {
        if (!uuid_) throw pending_reverse("BattleLogEntry needs LogicUuid");
        uuid_->encode(s);
        s.writeBoolean(b160_);
        s.writeVInt(v164_);
        s.writeVInt(v168_);
    } else {
        s.writeVInt(0); // absent -> final vint 0
    }
}

inline void BattleLogEntry::decode(ByteStream& s) {
    v0_ = s.readVInt();
    v36_ = s.readVInt();
    v4_ = s.readVInt();
    v8_ = s.readVInt();
    v12_ = s.readVInt();
    b16_ = s.readBoolean();
    ref_ = DataReference::decodeNullable(s);
    v32_ = s.readVInt();
    v40_ = s.readVInt();
    q48_ = s.readLongLong();
    q56_ = s.readLongLong();
    v64_ = s.readVInt();
    b68_ = s.readBoolean();
    const i32 n = s.readVInt();
    players_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto p = std::make_unique<BattleLogPlayerEntry>();
        p->decode(s);
        players_.push_back(std::move(p));
    }
    v72_ = s.readVInt();
    b76_ = s.readBoolean();
    v80_ = s.readVInt();
    b84_ = s.readBoolean();
    v88_ = s.readVInt();
    hasOpt112_ = s.readBoolean();
    if (hasOpt112_) {
        id112_ = LogicLong::decode(s);
        v120_ = s.readVInt();
        ref128_ = s.readStringReference();
        id144_ = LogicLong::decode(s);
    }
    hasOpt152_ = s.readBoolean();
    if (hasOpt152_) {
        uuid_ = std::make_unique<LogicUuid>();
        uuid_->decode(s);
        b160_ = s.readBoolean();
        v164_ = s.readVInt();
        v168_ = s.readVInt();
    } else {
        v168_ = s.readVInt(); // final vint 0
    }
}

} // namespace titan
