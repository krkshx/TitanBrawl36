// Message batch 00 bodies — reversed from libg_decrypted.so (ARM64).
// Nested entries throw pending_reverse until their own wave lands;
// empty arrays (count 0) already round-trip bit-exact.

#include "titan/messages/MsgBatch00.hpp"

namespace titan {

// ---- 20107 AddableFriendsMessage ----
void AddableFriendsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}
void AddableFriendsMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    entries_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<AddableFriendEntry>();
        e->decode(stream());
        entries_.push_back(std::move(e));
    }
}

// ---- 24301 AllianceDataMessage ----
void AllianceDataMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(isMyAlliance_);
    if (!entry_) throw pending_reverse("AllianceDataMessage needs AllianceFullEntry");
    entry_->encode(stream());
}
void AllianceDataMessage::decode() {
    PiranhaMessage::decode();
    isMyAlliance_ = stream().readBoolean();
    entry_ = std::make_unique<AllianceFullEntry>();
    entry_->decode(stream());
}

// ---- 24310 AllianceListMessage ----
void AllianceListMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(searchQuery_ ? &*searchQuery_ : nullptr);
    stream().writeVInt(static_cast<i32>(headers_.size()));
    for (const auto& h : headers_) h->encode(stream());
}
void AllianceListMessage::decode() {
    PiranhaMessage::decode();
    searchQuery_ = stream().readString();
    const i32 n = stream().readVInt();
    headers_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto h = std::make_unique<AllianceHeaderEntry>();
        h->decode(stream());
        headers_.push_back(std::move(h));
    }
}

// ---- 24308 AllianceMemberMessage ----
void AllianceMemberMessage::encode() {
    PiranhaMessage::encode();
    avatarId_.encode(stream());
    if (!entry_) throw pending_reverse("AllianceMemberMessage needs AllianceMemberEntry");
    entry_->encode(stream());
}
void AllianceMemberMessage::decode() {
    PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    entry_ = std::make_unique<AllianceMemberEntry>();
    entry_->decode(stream());
}

// ---- 20207 AllianceOnlineStatusUpdatedMessage ----
void AllianceOnlineStatusUpdatedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(statusType_);
    stream().writeVInt(static_cast<i32>(changes_.size()));
    for (const auto& c : changes_) c->encode(stream());
}
void AllianceOnlineStatusUpdatedMessage::decode() {
    PiranhaMessage::decode();
    statusType_ = stream().readVInt();
    const i32 n = stream().readVInt();
    changes_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto c = std::make_unique<StatusChangeEntry>();
        c->decode(stream());
        changes_.push_back(std::move(c));
    }
}

// ---- 24312 AllianceStreamEntryMessage ----
void AllianceStreamEntryMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(entryType_);
    if (!entry_) throw pending_reverse("AllianceStreamEntryMessage needs StreamEntry");
    entry_->encode(stream());
}
void AllianceStreamEntryMessage::decode() {
    PiranhaMessage::decode();
    entryType_ = stream().readVInt();
    entry_ = std::make_unique<StreamEntry>();
    entry_->decode(stream());
}

// ---- 24311 AllianceStreamMessage ----
void AllianceStreamMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& [type, e] : entries_) {
        stream().writeVInt(type);
        e->encode(stream());
    }
}
void AllianceStreamMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    entries_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        const i32 type = stream().readVInt();
        auto e = std::make_unique<StreamEntry>();
        e->decode(stream());
        entries_.emplace_back(type, std::move(e));
    }
}

// ---- 24364 AllianceTeamsMessage ----
void AllianceTeamsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    stream().writeVInt(static_cast<i32>(teams_.size()));
    for (const auto& t : teams_) t->encode(stream());
}
void AllianceTeamsMessage::decode() {
    PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    const i32 n = stream().readVInt();
    teams_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto t = std::make_unique<AllianceTeamEntry>();
        t->decode(stream());
        teams_.push_back(std::move(t));
    }
}

// ---- 24776 AllianceWarMessage ----
void AllianceWarMessage::encode() {
    PiranhaMessage::encode();
    allianceId_.encode(stream());
    stream().writeVInt(field144_);
    stream().writeVInt(static_cast<i32>(nodes_.size()));
    for (const auto& x : nodes_) x->encode(stream());
    stream().writeVInt(static_cast<i32>(factions_.size()));
    for (const auto& x : factions_) x->encode(stream());
}
void AllianceWarMessage::decode() {
    PiranhaMessage::decode();
    allianceId_ = LogicLong::decode(stream());
    field144_ = stream().readVInt();
    const i32 nn = stream().readVInt();
    nodes_.clear();
    for (i32 i = 0; i < nn; ++i) {
        auto x = std::make_unique<AllianceWarNode>();
        x->decode(stream());
        nodes_.push_back(std::move(x));
    }
    const i32 nf = stream().readVInt();
    factions_.clear();
    for (i32 i = 0; i < nf; ++i) {
        auto x = std::make_unique<AllianceWarFaction>();
        x->decode(stream());
        factions_.push_back(std::move(x));
    }
}

// ---- 10110 AnalyticEventMessage ----
void AnalyticEventMessage::encode() {
    PiranhaMessage::encode();
    if (!event_) throw pending_reverse("AnalyticEventMessage needs AnalyticEvent");
    event_->encode(stream());
}
void AnalyticEventMessage::decode() {
    PiranhaMessage::decode();
    event_ = std::make_unique<AnalyticEvent>();
    event_->decode(stream());
}

} // namespace titan
