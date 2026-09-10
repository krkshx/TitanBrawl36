// Message batch 04 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch04.hpp"
#include "titan/messages/MsgBatch01.hpp" // LogicCommand (complete type)

namespace titan {

// ---- 14102 EndClientTurnMessage ----
void EndClientTurnMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    stream().writeVInt(tick_);
    stream().writeVInt(checksum_);
    stream().writeVInt(static_cast<i32>(commands_.size()));
    for (const auto& c : commands_) c->encode(stream());
    if (payload_) {
        const auto& p = *payload_;
        stream().writeBytes(p.data(), static_cast<i32>(p.size()));
    } else {
        stream().writeBytes(nullptr, 0);
    }
}
void EndClientTurnMessage::decode() {
    PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    tick_ = stream().readVInt();
    checksum_ = stream().readVInt();
    const i32 n = stream().readVInt();
    commands_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto c = std::make_unique<LogicCommand>();
        c->decode(stream());
        commands_.push_back(std::move(c));
    }
    payload_ = stream().readBytesNullable();
}

// Wire: string id, bool hasAvatar, [logiclong], string name,
// [bool flag], LogicClientAvatar, [string extra].
void AccountAlreadyBoundBase::encodeBound(bool hasFlag, bool hasExtra) {
    stream().writeString(accountId_ ? &*accountId_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(avatarName_ ? &*avatarName_ : nullptr);
    if (hasFlag) stream().writeBoolean(flag_);
    if (!avatar_) throw pending_reverse("AccountAlreadyBound needs LogicClientAvatar");
    avatar_->encode(stream());
    if (hasExtra) stream().writeString(extra_ ? &*extra_ : nullptr);
}
void AccountAlreadyBoundBase::decodeBound(bool hasFlag, bool hasExtra) {
    accountId_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    avatarName_ = stream().readString();
    if (hasFlag) flag_ = stream().readBoolean();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    if (hasExtra) extra_ = stream().readString();
}

// ---- 20105 FriendListMessage ----
void FriendListMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(unknown_);
    stream().writeBoolean(flag1_);
    stream().writeBoolean(flag2_);
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(friends_.size()));
    for (const auto& f : friends_) f->encode(stream());
}
void FriendListMessage::decode() {
    PiranhaMessage::decode();
    unknown_ = stream().readInt();
    flag1_ = stream().readBoolean();
    flag2_ = stream().readBoolean();
    const i32 n = stream().readInt();
    friends_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        auto f = std::make_unique<FriendEntry>();
        f->decode(stream());
        friends_.push_back(std::move(f));
    }
}

// ---- 20106 FriendListUpdateMessage ----
void FriendListUpdateMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    if (!entry_) throw pending_reverse("FriendListUpdateMessage needs FriendEntry");
    entry_->encode(stream());
}
void FriendListUpdateMessage::decode() {
    PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    entry_ = std::make_unique<FriendEntry>();
    entry_->decode(stream());
}

// ---- 24555 FriendOnlineStatusEntryMessage ----
void FriendOnlineStatusEntryMessage::encode() {
    PiranhaMessage::encode();
    avatarId_.encode(stream());
    stream().writeBoolean(hasStatus_);
    if (hasStatus_) {
        if (!status_) throw pending_reverse("FriendOnlineStatusEntryMessage needs FriendOnlineStatus");
        status_->encode(stream());
    }
}
void FriendOnlineStatusEntryMessage::decode() {
    PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    hasStatus_ = stream().readBoolean();
    if (hasStatus_) {
        status_ = std::make_unique<FriendOnlineStatus>();
        status_->decode(stream());
    }
}

// ---- 20109 FriendOnlineStatusMessage ----
void FriendOnlineStatusMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(avatarIds_.size()));
    for (const auto& id : avatarIds_) id.encode(stream());
}
void FriendOnlineStatusMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    avatarIds_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) avatarIds_.push_back(LogicLong::decode(stream()));
}

// ---- 20199 FriendSuggestionsMessage ----
void FriendSuggestionsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(static_cast<i32>(suggestions_.size()));
    for (const auto& s : suggestions_) s->encode(stream());
}
void FriendSuggestionsMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    suggestions_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto s = std::make_unique<FriendSuggestionEntry>();
        s->decode(stream());
        suggestions_.push_back(std::move(s));
    }
}

// ---- 14113 GetPlayerProfileMessage ----
void GetPlayerProfileMessage::encode() {
    PiranhaMessage::encode();
    avatarId_.encode(stream());
    stream().writeBoolean(hasEntry_);
    if (hasEntry_) {
        if (!entry_) throw pending_reverse("GetPlayerProfileMessage needs BattleLogPlayerEntry");
        entry_->encode(stream());
    }
    stream().writeVInt(unknown_);
}
void GetPlayerProfileMessage::decode() {
    PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    hasEntry_ = stream().readBoolean();
    if (hasEntry_) {
        entry_ = std::make_unique<BattleLogPlayerEntry>();
        entry_->decode(stream());
    }
    unknown_ = stream().readVInt();
}

} // namespace titan
