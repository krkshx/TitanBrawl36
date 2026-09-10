// Message batch 10 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch10.hpp"

namespace titan {

// ---- 20559 StartLoadingMessage ----
void StartLoadingMessage::encode() {
    PiranhaMessage::encode();
    for (i32 x : head_) stream().writeInt(x);
    stream().writeInt(static_cast<i32>(players_.size()));
    for (const auto& p : players_) p->encode(stream());
    stream().writeInt(static_cast<i32>(spawns_.size()));
    for (const auto& s : spawns_) s->encode(stream());
    stream().writeInt(static_cast<i32>(unkInts_.size()));
    for (i32 x : unkInts_) stream().writeInt(x);
    stream().writeInt(unk144_);
    for (i32 x : vints_) stream().writeVInt(x);
    stream().writeVInt(v168_);
    DataReference::encodeNullable(stream(), mapRef_);
    if (!battleMap_) throw pending_reverse("StartLoadingMessage needs BattlePlayerMap");
    battleMap_->encode(stream());
    stream().writeBoolean(unk172_);
    stream().writeBoolean(unk173_);
    stream().writeVInt(v176_);
    stream().writeVInt(v180_);
}
void StartLoadingMessage::decode() {
    PiranhaMessage::decode();
    for (i32& x : head_) x = stream().readInt();
    const i32 np = stream().readInt();
    players_.clear();
    for (i32 i = 0; i < np; ++i) {
        auto p = std::make_unique<LogicPlayer>();
        p->decode(stream());
        players_.push_back(std::move(p));
    }
    const i32 ns = stream().readInt();
    spawns_.clear();
    for (i32 i = 0; i < ns; ++i) {
        auto s = std::make_unique<LogicVector2>();
        s->decode(stream());
        spawns_.push_back(std::move(s));
    }
    const i32 ni = stream().readInt();
    unkInts_.clear();
    for (i32 i = 0; i < ni; ++i) unkInts_.push_back(stream().readInt());
    unk144_ = stream().readInt();
    for (i32& x : vints_) x = stream().readVInt();
    v168_ = stream().readVInt();
    mapRef_ = DataReference::decodeNullable(stream());
    battleMap_ = std::make_unique<BattlePlayerMap>();
    battleMap_->decode(stream());
    unk172_ = stream().readBoolean();
    unk173_ = stream().readBoolean();
    v176_ = stream().readVInt();
    v180_ = stream().readVInt();
}

// ---- 14350 TeamCreateMessage ----
void TeamCreateMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v136_);
    stream().writeVInt(v140_);
    stream().writeVInt(teamType_);
    stream().writeBoolean(unk144_);
    stream().writeBoolean(hasInvite_);
    if (hasInvite_) {
        inviteId_.encode(stream());
        stream().writeVInt(unk160_);
        DataReference::encodeNullable(stream(), eventRef_);
    }
    roomId_.encode(stream());
}
void TeamCreateMessage::decode() {
    PiranhaMessage::decode();
    v136_ = stream().readVInt();
    v140_ = stream().readVInt();
    teamType_ = stream().readVInt();
    unk144_ = stream().readBoolean();
    hasInvite_ = stream().readBoolean();
    if (hasInvite_) {
        inviteId_ = LogicLong::decode(stream());
        unk160_ = stream().readVInt();
        eventRef_ = DataReference::decodeNullable(stream());
    }
    roomId_ = LogicLong::decode(stream());
}

// ---- 24589 TeamInvitationMessage ----
void TeamInvitationMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(slotId_);
    if (!invitation_) throw pending_reverse("TeamInvitationMessage needs TeamInvitation");
    invitation_->encode(stream());
}
void TeamInvitationMessage::decode() {
    PiranhaMessage::decode();
    slotId_ = stream().readVInt();
    invitation_ = std::make_unique<TeamInvitation>();
    invitation_->decode(stream());
}

// ---- 24582 TeamInviteStatusMessage ----
void TeamInviteStatusMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(statusCode_);
    stream().writeBoolean(entry_ != nullptr);
    if (entry_) entry_->encode(stream());
}
void TeamInviteStatusMessage::decode() {
    PiranhaMessage::decode();
    statusCode_ = stream().readInt();
    if (stream().readBoolean()) {
        entry_ = std::make_unique<TeamInvitationDataEntry>();
        entry_->decode(stream());
    }
}

} // namespace titan
