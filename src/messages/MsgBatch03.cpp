// Message batch 03 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch03.hpp"

namespace titan {

// ---- AllianceSettingsBase (14316 + 14301) ----
// Wire: string name, string description, dataref badge, dataref region,
// vint allianceType, vint requiredTrophies, bool familyFriendly.
void AllianceSettingsBase::encodeSettings() {
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeString(description_ ? &*description_ : nullptr);
    DataReference::encodeNullable(stream(), badge_);
    DataReference::encodeNullable(stream(), region_);
    stream().writeVInt(allianceType_);
    stream().writeVInt(requiredTrophies_);
    stream().writeBoolean(familyFriendly_);
}
void AllianceSettingsBase::decodeSettings() {
    name_ = stream().readString();
    description_ = stream().readString();
    badge_ = DataReference::decodeNullable(stream());
    region_ = DataReference::decodeNullable(stream());
    allianceType_ = stream().readVInt();
    requiredTrophies_ = stream().readVInt();
    familyFriendly_ = stream().readBoolean();
}

// ---- 22100 CreatePlayerMapResponseMessage ----
void CreatePlayerMapResponseMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(resultCode_);
    stream().writeBoolean(hasMap_);
    if (hasMap_) {
        if (!map_) throw pending_reverse("CreatePlayerMapResponseMessage needs LogicPlayerMap");
        map_->encode(stream());
    }
}
void CreatePlayerMapResponseMessage::decode() {
    PiranhaMessage::decode();
    resultCode_ = stream().readVInt();
    hasMap_ = stream().readBoolean();
    if (hasMap_) {
        map_ = std::make_unique<LogicPlayerMap>();
        map_->decode(stream());
    }
}

// ---- 24123 DailyEventsMessage ----
void DailyEventsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(f132_);
    stream().writeVInt(f136_);
    stream().writeVInt(static_cast<i32>(events_.size()));
    for (const auto& e : events_) e->encode(stream());
}
void DailyEventsMessage::decode() {
    PiranhaMessage::decode();
    f132_ = stream().readVInt();
    f136_ = stream().readVInt();
    const i32 n = stream().readVInt();
    events_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<EventData>();
        e->decode(stream());
        events_.push_back(std::move(e));
    }
}

} // namespace titan
