// Message batch 07 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch07.hpp"
#include "titan/messages/MsgBatch04.hpp" // LogicClientAvatar (complete type)

namespace titan {

// ---- 24399 MyAllianceMessage ----
void MyAllianceMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(f132_);
    stream().writeBoolean(hasAlliance_);
    if (hasAlliance_) {
        DataReference::encodeNullable(stream(), allianceRef_);
        if (!header_) throw pending_reverse("MyAllianceMessage needs AllianceHeaderEntry");
        header_->encode(stream());
    }
}
void MyAllianceMessage::decode() {
    PiranhaMessage::decode();
    f132_ = stream().readVInt();
    hasAlliance_ = stream().readBoolean();
    if (hasAlliance_) {
        allianceRef_ = DataReference::decodeNullable(stream());
        header_ = std::make_unique<AllianceHeaderEntry>();
        header_->decode(stream());
    }
}

// ---- 20801 NotificationMessage ----
void NotificationMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(notifType_);
    if (!payload_) throw pending_reverse("NotificationMessage needs Notification");
    payload_->encode(stream());
}
void NotificationMessage::decode() {
    PiranhaMessage::decode();
    notifType_ = stream().readVInt();
    payload_ = std::make_unique<Notification>();
    payload_->decode(stream());
}

// ---- 24101 OwnHomeDataMessage ----
void OwnHomeDataMessage::encode() {
    PiranhaMessage::encode();
    if (!home_) throw pending_reverse("OwnHomeDataMessage needs LogicClientHome");
    home_->encode(stream());
    if (!avatar_) throw pending_reverse("OwnHomeDataMessage needs LogicClientAvatar");
    avatar_->encode(stream());
    stream().writeVInt(f152_);
}
void OwnHomeDataMessage::decode() {
    PiranhaMessage::decode();
    home_ = std::make_unique<LogicClientHome>();
    home_->decode(stream());
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    f152_ = stream().readVInt();
}

// ---- 22102 PlayerMapsMessage ----
void PlayerMapsMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(maps_.size()));
    for (const auto& m : maps_) m->encode(stream());
}
void PlayerMapsMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    maps_.clear();
    isNull_ = (n < 0);
    if (isNull_) return; // binary treats null as empty
    for (i32 i = 0; i < n; ++i) {
        auto m = std::make_unique<LogicPlayerMap>();
        m->decode(stream());
        maps_.push_back(std::move(m));
    }
}

// ---- 24113 PlayerProfileMessage ----
void PlayerProfileMessage::encode() {
    PiranhaMessage::encode();
    if (!profile_) throw pending_reverse("PlayerProfileMessage needs PlayerProfile");
    profile_->encode(stream());
    stream().writeBoolean(hasAlliance_);
    if (hasAlliance_) {
        if (!alliance_) throw pending_reverse("PlayerProfileMessage needs AllianceHeaderEntry");
        alliance_->encode(stream());
    }
    DataReference::encodeNullable(stream(), ref152_);
}
void PlayerProfileMessage::decode() {
    PiranhaMessage::decode();
    profile_ = std::make_unique<PlayerProfile>();
    profile_->decode(stream());
    hasAlliance_ = stream().readBoolean();
    if (hasAlliance_) {
        alliance_ = std::make_unique<AllianceHeaderEntry>();
        alliance_->decode(stream());
    }
    ref152_ = DataReference::decodeNullable(stream());
}

// ---- 22153 RankedMatchBanEndedMessage (LogicMapInt: count + key/ref pairs) ----
void RankedMatchBanEndedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(static_cast<i32>(bans_.size()));
    for (const auto& [key, ref] : bans_) {
        stream().writeVInt(key);
        ref.encode(stream());
    }
}
void RankedMatchBanEndedMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    bans_.clear();
    for (i32 i = 0; i < n; ++i) {
        const i32 key = stream().readVInt();
        DataReference ref;
        ref.classId = stream().readVInt();
        ref.instanceId = stream().readVInt();
        bans_.emplace_back(key, ref);
    }
}

} // namespace titan
