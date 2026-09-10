// Message batch 06 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch06.hpp"

namespace titan {

// ---- 29003 LatencyTestStatusMessage ----
void LatencyTestStatusMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}
void LatencyTestStatusMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    entries_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<LatencyData>();
        e->decode(stream());
        entries_.push_back(std::move(e));
    }
}

// ---- 24403 LeaderboardMessage ----
void LeaderboardMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeVInt(v136_);
    DataReference::encodeNullable(stream(), ref_);
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeVInt(static_cast<i32>(rankings_.size()));
    for (const auto& r : rankings_) r->encode(stream());
    for (i32 x : tail_) stream().writeVInt(x);
    stream().writeString(tailStr_ ? &*tailStr_ : nullptr);
}
void LeaderboardMessage::decode() {
    PiranhaMessage::decode();
    v132_ = stream().readVInt();
    v136_ = stream().readVInt();
    ref_ = DataReference::decodeNullable(stream());
    name_ = stream().readString();
    const i32 n = stream().readVInt();
    rankings_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto r = std::make_unique<RankingEntry>();
        r->decode(stream());
        rankings_.push_back(std::move(r));
    }
    for (i32& x : tail_) x = stream().readVInt();
    tailStr_ = stream().readString();
}

// ---- 24205 LineAccountAlreadyBoundMessage ----
// Wire: string id, bool hasId, [logiclong], string name, LogicClientAvatar.
void LineAccountAlreadyBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(id_ ? &*id_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("LineAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
}
void LineAccountAlreadyBoundMessage::decode() {
    PiranhaMessage::decode();
    id_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
}

// ---- 23457 LobbyInfoMessage ----
void LobbyInfoMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}
void LobbyInfoMessage::decode() {
    PiranhaMessage::decode();
    v132_ = stream().readVInt();
    name_ = stream().readString();
    const i32 n = stream().readVInt();
    entries_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<LobbyInfoEntry>();
        e->decode(stream());
        entries_.push_back(std::move(e));
    }
}

// ---- 20104 LoginOkMessage ----
void LoginOkMessage::encode() {
    PiranhaMessage::encode();
    accountId_.encode(stream());
    homeId_.encode(stream());
    stream().writeString(passToken_ ? &*passToken_ : nullptr);
    stream().writeString(f160_ ? &*f160_ : nullptr);
    stream().writeString(f168_ ? &*f168_ : nullptr);
    stream().writeInt(f192_);
    stream().writeInt(f196_);
    stream().writeInt(f200_);
    stream().writeString(f208_ ? &*f208_ : nullptr);
    stream().writeInt(f216_);
    stream().writeInt(f220_);
    stream().writeInt(f224_);
    stream().writeString(f184_ ? &*f184_ : nullptr);
    // Guarded tail is always written by encode; decode tolerates absence.
    stream().writeString(f232_ ? &*f232_ : nullptr);
    stream().writeString(f240_ ? &*f240_ : nullptr);
    stream().writeInt(f248_);
    stream().writeString(f176_ ? &*f176_ : nullptr);
    stream().writeString(f256_ ? &*f256_ : nullptr);
    stream().writeString(f264_ ? &*f264_ : nullptr);
    stream().writeInt(f312_);
    stream().writeString(f272_ ? &*f272_ : nullptr);
    stream().writeInt(static_cast<i32>(list296_.size()));
    for (const auto& s : list296_) stream().writeString(&s);
    stream().writeInt(static_cast<i32>(list304_.size()));
    for (const auto& s : list304_) stream().writeString(&s);
    stream().writeVInt(f332_);
    if (!compressed_) throw pending_reverse("LoginOkMessage needs LogicCompressedString");
    compressed_->encode(stream());
    stream().writeBoolean(f328_);
    stream().writeBoolean(!f329not_);
    stream().writeString(f280_ ? &*f280_ : nullptr);
    stream().writeString(f336_ ? &*f336_ : nullptr);
    stream().writeString(f288_ ? &*f288_ : nullptr);
    stream().writeString(f344_ ? &*f344_ : nullptr);
    stream().writeString(f352_ ? &*f352_ : nullptr);
    stream().writeBoolean(f360_);
}

static std::vector<std::string> readCappedStringList(ByteStream& s) {
    const i32 n = s.readInt();
    std::vector<std::string> out;
    for (i32 i = 0; i < n && i < 100; ++i) out.push_back(s.readString().value_or(""));
    return out;
}

void LoginOkMessage::decode() {
    PiranhaMessage::decode();
    accountId_ = LogicLong::decode(stream());
    homeId_ = LogicLong::decode(stream());
    passToken_ = stream().readString();
    f160_ = stream().readString();
    f168_ = stream().readString();
    f192_ = stream().readInt();
    f196_ = stream().readInt();
    f200_ = stream().readInt();
    f208_ = stream().readString();
    f216_ = stream().readInt();
    f220_ = stream().readInt();
    f224_ = stream().readInt();
    f184_ = stream().readString();
    if (stream().isAtEnd()) return;
    f232_ = stream().readString();
    f240_ = stream().readString();
    f248_ = stream().readInt();
    hasTail1_ = true;
    if (stream().isAtEnd()) return;
    f176_ = stream().readString();
    f256_ = stream().readString();
    f264_ = stream().readString();
    f312_ = stream().readInt();
    hasTail2_ = true;
    if (stream().isAtEnd()) return;
    f272_ = stream().readString();
    list296_ = readCappedStringList(stream());
    list304_ = readCappedStringList(stream());
    hasLists_ = true;
    if (stream().isAtEnd()) return;
    f332_ = stream().readVInt();
    hasF332_ = true;
    if (stream().isAtEnd()) return;
    compressed_ = std::make_unique<LogicCompressedString>();
    compressed_->decode(stream());
    hasCompressed_ = true;
    if (stream().isAtEnd()) return;
    f328_ = stream().readBoolean();
    f329not_ = !stream().readBoolean();
    hasFlags_ = true;
    if (stream().isAtEnd()) return;
    f280_ = stream().readString();
    f336_ = stream().readString();
    f288_ = stream().readString();
    f344_ = stream().readString();
    f352_ = stream().readString();
    f360_ = stream().readBoolean();
    hasTail3_ = true;
}

} // namespace titan
