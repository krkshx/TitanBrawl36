// Message batch 08 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch08.hpp"

namespace titan {

static void encodeOptionalPlayer(ByteStream& s, bool has,
                                 const std::unique_ptr<LogicRankedMatchPlayer>& p) {
    s.writeBoolean(has);
    if (has) {
        if (!p) throw pending_reverse("ranked message needs LogicRankedMatchPlayer");
        p->encode(s);
    }
}
static void decodeOptionalPlayer(ByteStream& s, bool& has,
                                 std::unique_ptr<LogicRankedMatchPlayer>& p) {
    has = s.readBoolean();
    if (has) {
        p = std::make_unique<LogicRankedMatchPlayer>();
        p->decode(s);
    }
}

// ---- 22157 RankedMatchHeroDataUpdatedMessage ----
void RankedMatchHeroDataUpdatedMessage::encode() {
    PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
}
void RankedMatchHeroDataUpdatedMessage::decode() {
    PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
}

// ---- 22156 RankedMatchHeroPickedMessage ----
void RankedMatchHeroPickedMessage::encode() {
    PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
}
void RankedMatchHeroPickedMessage::decode() {
    PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
}

// ---- 22154 RankedMatchPickStartedMessage ----
void RankedMatchPickStartedMessage::encode() {
    PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
    stream().writeVInt(v144_);
    stream().writeVInt(v148_);
    stream().writeVInt(v152_);
    id156_.encode(stream());
}
void RankedMatchPickStartedMessage::decode() {
    PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
    v144_ = stream().readVInt();
    v148_ = stream().readVInt();
    v152_ = stream().readVInt();
    id156_ = LogicLong::decode(stream());
}

// ---- 22150 RankedMatchStartedMessage ----
void RankedMatchStartedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(hasMatch_);
    if (hasMatch_) {
        if (!match_) throw pending_reverse("RankedMatchStartedMessage needs LogicRankedMatch");
        match_->encode(stream());
    }
}
void RankedMatchStartedMessage::decode() {
    PiranhaMessage::decode();
    hasMatch_ = stream().readBoolean();
    if (hasMatch_) {
        match_ = std::make_unique<LogicRankedMatch>();
        match_->decode(stream());
    }
}

// ---- 28689 SCIDAccountAlreadyBoundMessage ----
// Wire: string token, bool hasId, [logiclong], string name,
// LogicClientAvatar, string extra.
void SCIDAccountAlreadyBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(token_ ? &*token_ : nullptr);
    stream().writeBoolean(hasPlayerId_);
    if (hasPlayerId_) playerId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("SCIDAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
    stream().writeString(extra_ ? &*extra_ : nullptr);
}
void SCIDAccountAlreadyBoundMessage::decode() {
    PiranhaMessage::decode();
    token_ = stream().readString();
    hasPlayerId_ = stream().readBoolean();
    if (hasPlayerId_) playerId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    extra_ = stream().readString();
}

static void encodeCompressed(ByteStream& s, const std::unique_ptr<LogicCompressedString>& v,
                             const char* ctx) {
    if (!v) throw pending_reverse(ctx);
    v->encode(s);
}
static void decodeCompressed(ByteStream& s, std::unique_ptr<LogicCompressedString>& v) {
    v = std::make_unique<LogicCompressedString>();
    v->decode(s);
}

// ---- 25165 SCIDAccountBoundMessage ----
void SCIDAccountBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(result_);
    encodeCompressed(stream(), data_, "SCIDAccountBoundMessage needs LogicCompressedString");
}
void SCIDAccountBoundMessage::decode() {
    PiranhaMessage::decode();
    result_ = stream().readInt();
    decodeCompressed(stream(), data_);
}

// ---- 10636 SCIDBindAccountMessage ----
void SCIDBindAccountMessage::encode() {
    PiranhaMessage::encode();
    encodeCompressed(stream(), f1_, "SCIDBindAccountMessage needs LogicCompressedString");
    encodeCompressed(stream(), f2_, "SCIDBindAccountMessage needs LogicCompressedString");
}
void SCIDBindAccountMessage::decode() {
    PiranhaMessage::decode();
    decodeCompressed(stream(), f1_);
    decodeCompressed(stream(), f2_);
}

// ---- 11736 SCIDLogoutAllDevicesMessage ----
void SCIDLogoutAllDevicesMessage::encode() {
    PiranhaMessage::encode();
    encodeCompressed(stream(), token_, "SCIDLogoutAllDevicesMessage needs LogicCompressedString");
}
void SCIDLogoutAllDevicesMessage::decode() {
    PiranhaMessage::decode();
    decodeCompressed(stream(), token_);
}

} // namespace titan
