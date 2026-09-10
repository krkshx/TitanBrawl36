// Shared message base bodies — split out of MsgBatch01/02/03.cpp.

#include "titan/messages/Common.hpp"

namespace titan {


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


void PlayingFriendsQuery::encodeIds(bool useVInt, bool useRef) {
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
void PlayingFriendsQuery::decodeIds(bool useVInt, bool useRef) {
    const i32 n = useVInt ? stream().readVInt() : stream().readInt();
    ids_.clear();
    null_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        ids_.push_back(useRef ? stream().readStringReference()
                              : stream().readString().value_or(""));
    }
}


void BindAccountMessage::encodeBind() {
    if (hasForce_) stream().writeBoolean(force_);
    stream().writeString(accountId_ ? &*accountId_ : nullptr);
    stream().writeString(token_ ? &*token_ : nullptr);
    if (blobCount_ == 3)
        stream().writeString(extra_ ? &*extra_ : nullptr);
    for (const auto& b : blobs_)
        stream().writeBytes(b.empty() ? nullptr : b.data(), static_cast<i32>(b.size()));
    if (hasVintTail_) stream().writeVInt(vintTail_);
}
void BindAccountMessage::decodeBind() {
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

} // namespace titan
