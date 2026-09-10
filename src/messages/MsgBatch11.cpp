// Message batch 11 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch11.hpp"

namespace titan {

static void encodeIntList(ByteStream& s, const std::vector<i32>& v) {
    s.writeVInt(static_cast<i32>(v.size()));
    for (i32 x : v) s.writeVInt(x);
}
static void decodeIntList(ByteStream& s, std::vector<i32>& v) {
    const i32 n = s.readVInt();
    v.clear();
    for (i32 i = 0; i < n; ++i) v.push_back(s.readVInt());
}

// ---- 14363 TeamSetLocationMessage ----
void TeamSetLocationMessage::encode() {
    PiranhaMessage::encode();
    DataReference::encodeNullable(stream(), locationRef_);
    stream().writeVInt(v144_);
    encodeIntList(stream(), intList_);
}
void TeamSetLocationMessage::decode() {
    PiranhaMessage::decode();
    locationRef_ = DataReference::decodeNullable(stream());
    v144_ = stream().readVInt();
    decodeIntList(stream(), intList_);
}

// ---- 12110 TeamSetPlayerMapMessage ----
void TeamSetPlayerMapMessage::encode() {
    PiranhaMessage::encode();
    mapId_.encode(stream());
    encodeIntList(stream(), intList_);
}
void TeamSetPlayerMapMessage::decode() {
    PiranhaMessage::decode();
    mapId_ = LogicLong::decode(stream());
    decodeIntList(stream(), intList_);
}

// ---- 24131 TeamStreamMessage ----
void TeamStreamMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeVInt(v136_);
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
void TeamStreamMessage::decode() {
    PiranhaMessage::decode();
    v132_ = stream().readVInt();
    v136_ = stream().readVInt();
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

// ---- 24221 TencentAccountAlreadyBoundMessage ----
void TencentAccountAlreadyBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(id_ ? &*id_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("TencentAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
}
void TencentAccountAlreadyBoundMessage::decode() {
    PiranhaMessage::decode();
    id_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
}

// ---- 14267 TencentAntiAddictionInstructionExecutedMessage ----
void TencentAntiAddictionInstructionExecutedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeStringReference(name_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(args_.size()));
    for (const auto& a : args_) stream().writeString(&a);
}
void TencentAntiAddictionInstructionExecutedMessage::decode() {
    PiranhaMessage::decode();
    name_ = stream().readStringReference();
    const i32 n = stream().readVInt();
    args_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n && i < 11; ++i) args_.push_back(stream().readString().value_or(""));
}

// ---- 24222 TencentAntiAddictionInstructionMessage ----
void TencentAntiAddictionInstructionMessage::encode() {
    PiranhaMessage::encode();
    stream().writeStringReference(name_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(instructions_.size()));
    for (const auto& x : instructions_) x->encode(stream());
}
void TencentAntiAddictionInstructionMessage::decode() {
    PiranhaMessage::decode();
    name_ = stream().readStringReference();
    const i32 n = stream().readVInt();
    instructions_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        auto x = std::make_unique<LogicTencentAntiAddictionInstruction>();
        x->decode(stream());
        instructions_.push_back(std::move(x));
    }
}

} // namespace titan
