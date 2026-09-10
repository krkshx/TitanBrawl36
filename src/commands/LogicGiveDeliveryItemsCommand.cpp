// LogicGiveDeliveryItemsCommand bodies (203) — split out; unchanged.

#include "titan/commands/LogicGiveDeliveryItemsCommand.hpp"

namespace titan {

void LogicGiveDeliveryItemsCommand::encode(ByteStream& s) const {
    LogicCommand::encode(s);
    s.writeVInt(v1_);
    s.writeVInt(static_cast<i32>(units_.size()));
    for (const auto& u : units_) u->encode(s);
    s.writeBoolean(hasDrops_);
    if (hasDrops_) {
        if (!drops_) throw pending_reverse("203 needs ForcedDrops");
        drops_->encode(s);
    }
    s.writeVInt(v2_);
    s.writeVInt(v3_);
    s.writeVInt(v4_);
    s.writeBoolean(b1_);
    s.writeBoolean(b2_);
}

void LogicGiveDeliveryItemsCommand::decode(ByteStream& s) {
    LogicCommand::decode(s);
    v1_ = s.readVInt();
    const i32 n = s.readVInt();
    units_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto u = std::make_unique<DeliveryUnit>();
        u->decode(s);
        units_.push_back(std::move(u));
    }
    hasDrops_ = s.readBoolean();
    if (hasDrops_) {
        drops_ = std::make_unique<ForcedDrops>();
        drops_->decode(s);
    }
    v2_ = s.readVInt();
    v3_ = s.readVInt();
    v4_ = s.readVInt();
    b1_ = s.readBoolean();
    b2_ = s.readBoolean();
}

} // namespace titan
