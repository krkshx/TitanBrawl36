// BattleLogEntry bodies (see BattleLogEntry.hpp).

#include "titan/game/BattleLogEntry.hpp"

namespace titan {

void BattleLogEntry::encode(ByteStream& s) const {
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

void BattleLogEntry::decode(ByteStream& s) {
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
