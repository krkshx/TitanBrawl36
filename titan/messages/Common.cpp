#pragma once

// Shared message bases and entry-array helpers.
// Split out of the old MsgBatch*.hpp files so every message owns its file.

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/core/PiranhaMessage.cpp"
#include "titan/game/avatar/LogicClientAvatar.cpp"
#include "titan/game/util/LogicCompressedString.cpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/battle/LogicRankedMatchPlayer.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

// FriendAvatarBaseMessage::encode @0x672c90: two writeInt (idHigh, idLow).
struct FriendAvatarBase {
    i32 idHigh = 0;
    i32 idLow = 0;
    void encode(ByteStream& s) const {
        s.writeInt(idHigh);
        s.writeInt(idLow);
    }
    void decode(ByteStream& s) {
        idHigh = s.readInt();
        idLow = s.readInt();
    }
};

// Shared shape of ChangeAllianceSettingsMessage (14316) and
// CreateAllianceMessage (14301).
// Wire: string name, string description, dataref badge, dataref region,
// vint allianceType, vint requiredTrophies, bool familyFriendly.
class AllianceSettingsBase : public PiranhaMessage {
public:
    void encodeSettings();
    void decodeSettings();
    std::optional<std::string> name_, description_;
    std::optional<DataReference> badge_, region_;
    i32 allianceType_ = 0, requiredTrophies_ = 0;
    bool familyFriendly_ = false;
};

// Shared shape of the AskForPlaying*Friends family (count + id list).
class PlayingFriendsQuery : public PiranhaMessage {
public:
    void encodeIds(bool useVInt, bool useRef);
    void decodeIds(bool useVInt, bool useRef);
    std::vector<std::string> ids_;
    bool null_ = false;
};

// Shared shape of the Bind*Account family.
class BindAccountMessage : public PiranhaMessage {
public:
    void encodeBind();
    void decodeBind();
    bool hasForce_ = true;
    bool force_ = false;
    std::optional<std::string> accountId_, token_, extra_;
    std::vector<std::vector<u8>> blobs_;
    i32 vintTail_ = 0;
    bool hasVintTail_ = false;
    int blobCount_ = 0; // 0 or 3 depending on subclass
};

// Shared shape for *AccountAlreadyBound: string id, bool hasAvatar,
// [logiclong], string name, [bool flag], LogicClientAvatar, [string extra].
class AccountAlreadyBoundBase : public PiranhaMessage {
public:
    void encodeBound(bool hasFlag, bool hasExtra);
    void decodeBound(bool hasFlag, bool hasExtra);
    std::optional<std::string> accountId_, avatarName_, extra_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    bool flag_ = false;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

// VInt-counted arrays of nested entries.
template <typename T>
void encodeEntryArray(ByteStream& s, const std::vector<std::unique_ptr<T>>& v) {
    s.writeVInt(static_cast<i32>(v.size()));
    for (const auto& e : v) e->encode(s);
}
template <typename T, typename Make>
void decodeEntryArray(ByteStream& s, std::vector<std::unique_ptr<T>>& v, Make make) {
    const i32 n = s.readVInt();
    v.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = make();
        e->decode(s);
        v.push_back(std::move(e));
    }
}

// VInt-counted arrays of plain vints (TeamSetLocation/TeamSetPlayerMap;
// wire item type flagged for verify-on-capture).
inline void encodeIntList(ByteStream& s, const std::vector<i32>& v) {
    s.writeVInt(static_cast<i32>(v.size()));
    for (i32 x : v) s.writeVInt(x);
}
inline void decodeIntList(ByteStream& s, std::vector<i32>& v) {
    const i32 n = s.readVInt();
    v.clear();
    for (i32 i = 0; i < n; ++i) v.push_back(s.readVInt());
}

// Optional LogicRankedMatchPlayer fields (ranked messages).
inline void encodeOptionalPlayer(ByteStream& s, bool has,
                                 const std::unique_ptr<LogicRankedMatchPlayer>& p) {
    s.writeBoolean(has);
    if (has) {
        if (!p) throw pending_reverse("ranked message needs LogicRankedMatchPlayer");
        p->encode(s);
    }
}
inline void decodeOptionalPlayer(ByteStream& s, bool& has,
                                 std::unique_ptr<LogicRankedMatchPlayer>& p) {
    has = s.readBoolean();
    if (has) {
        p = std::make_unique<LogicRankedMatchPlayer>();
        p->decode(s);
    }
}

// Optional LogicCompressedString fields (SCID messages).
inline void encodeCompressed(ByteStream& s,
                             const std::unique_ptr<LogicCompressedString>& v,
                             const char* ctx) {
    if (!v) throw pending_reverse(ctx);
    v->encode(s);
}
inline void decodeCompressed(ByteStream& s,
                             std::unique_ptr<LogicCompressedString>& v) {
    v = std::make_unique<LogicCompressedString>();
    v->decode(s);
}

} // namespace titan

// Shared message base bodies — split out of MsgBatch01/02/03.cpp.


namespace titan {


inline void AllianceSettingsBase::encodeSettings() {
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeString(description_ ? &*description_ : nullptr);
DataReference::encodeNullable(stream(), badge_);
DataReference::encodeNullable(stream(), region_);
    stream().writeVInt(allianceType_);
    stream().writeVInt(requiredTrophies_);
    stream().writeBoolean(familyFriendly_);
}
inline void AllianceSettingsBase::decodeSettings() {
    name_ = stream().readString();
    description_ = stream().readString();
    badge_ = DataReference::decodeNullable(stream());
    region_ = DataReference::decodeNullable(stream());
    allianceType_ = stream().readVInt();
    requiredTrophies_ = stream().readVInt();
    familyFriendly_ = stream().readBoolean();
}


inline void PlayingFriendsQuery::encodeIds(bool useVInt, bool useRef) {
    if (null_) {
        if (useVInt) stream().writeVInt(-1);
        else stream().writeInt(-1);
        return;
    }
    if (useVInt) stream().writeVInt(static_cast<i32>(ids_.size()));
    else stream().writeInt(static_cast<i32>(ids_.size()));
    for (const auto& id : ids_) {
        if (useRef) stream().writeStringReference(id);
        else stream().writeString(&id);
    }
}
inline void PlayingFriendsQuery::decodeIds(bool useVInt, bool useRef) {
    const i32 n = useVInt ? stream().readVInt() : stream().readInt();
    ids_.clear();
    null_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        ids_.push_back(useRef ? stream().readStringReference()
                              : stream().readString().value_or(""));
    }
}


inline void BindAccountMessage::encodeBind() {
    if (hasForce_) stream().writeBoolean(force_);
    stream().writeString(accountId_ ? &*accountId_ : nullptr);
    stream().writeString(token_ ? &*token_ : nullptr);
    if (blobCount_ == 3)
        stream().writeString(extra_ ? &*extra_ : nullptr);
    for (const auto& b : blobs_)
        stream().writeBytes(b.empty() ? nullptr : b.data(), static_cast<i32>(b.size()));
    if (hasVintTail_) stream().writeVInt(vintTail_);
}
inline void BindAccountMessage::decodeBind() {
    if (hasForce_) force_ = stream().readBoolean();
    accountId_ = stream().readString();
    token_ = stream().readString();
    if (blobCount_ == 3) extra_ = stream().readString();
    blobs_.clear();
    for (int i = 0; i < blobCount_; ++i) blobs_.push_back(stream().readBytes());
    if (hasVintTail_) vintTail_ = stream().readVInt();
}

// Wire: string id, bool hasAvatar, [logiclong], string name,
// [bool flag], LogicClientAvatar, [string extra].
inline void AccountAlreadyBoundBase::encodeBound(bool hasFlag, bool hasExtra) {
    stream().writeString(accountId_ ? &*accountId_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(avatarName_ ? &*avatarName_ : nullptr);
    if (hasFlag) stream().writeBoolean(flag_);
    if (!avatar_) throw pending_reverse("AccountAlreadyBound needs LogicClientAvatar");
    avatar_->encode(stream());
    if (hasExtra) stream().writeString(extra_ ? &*extra_ : nullptr);
}
inline void AccountAlreadyBoundBase::decodeBound(bool hasFlag, bool hasExtra) {
    accountId_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    avatarName_ = stream().readString();
    if (hasFlag) flag_ = stream().readBoolean();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    if (hasExtra) extra_ = stream().readString();
}

} // namespace titan

