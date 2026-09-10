// Message batch 02 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch02.hpp"

namespace titan {

// ---- 24412 AvatarStreamEntryMessage ----
void AvatarStreamEntryMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(entryType_);
    if (!entry_) throw pending_reverse("AvatarStreamEntryMessage needs AvatarStreamEntry");
    entry_->encode(stream());
}
void AvatarStreamEntryMessage::decode() {
    PiranhaMessage::decode();
    entryType_ = stream().readVInt();
    entry_ = std::make_unique<AvatarStreamEntry>();
    entry_->decode(stream());
}

// ---- 24411 AvatarStreamMessage ----
void AvatarStreamMessage::encode() {
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
void AvatarStreamMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    entries_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        const i32 type = stream().readVInt();
        auto e = std::make_unique<AvatarStreamEntry>();
        e->decode(stream());
        entries_.emplace_back(type, std::move(e));
    }
}

template <typename T>
static void encodeEntryArray(ByteStream& s, const std::vector<std::unique_ptr<T>>& v) {
    s.writeVInt(static_cast<i32>(v.size()));
    for (const auto& e : v) e->encode(s);
}
template <typename T, typename Make>
static void decodeEntryArray(ByteStream& s, std::vector<std::unique_ptr<T>>& v, Make make) {
    const i32 n = s.readVInt();
    v.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = make();
        e->decode(s);
        v.push_back(std::move(e));
    }
}

// ---- 23456 BattleEndMessage ----
void BattleEndMessage::encode() {
    PiranhaMessage::encode();
    stream().writeLongLong(battleIdA_);
    stream().writeLongLong(battleIdB_);
    for (i32 x : v11_) stream().writeVInt(x);
    stream().writeBoolean(hasGemOffer_);
    if (hasGemOffer_) {
        if (!gemOffer_) throw pending_reverse("BattleEndMessage needs LogicGemOffer");
        gemOffer_->encode(stream());
    }
    stream().writeVInt(v196_);
    stream().writeVInt(v200_);
    stream().writeBoolean(b204_);
    stream().writeVInt(v216_);
    stream().writeVInt(v220_);
    for (bool f : flags_) stream().writeBoolean(f);
    stream().writeVInt(v232_);
    stream().writeBoolean(b236_);
    encodeEntryArray(stream(), players_);
    encodeEntryArray(stream(), xp_);
    stream().writeVInt(static_cast<i32>(dataRefs_.size()));
    for (const auto& d : dataRefs_) d.encode(stream());
    encodeEntryArray(stream(), milestones_);
    DataReference::encodeNullable(stream(), dataRef272_);
    stream().writeBoolean(hasPlayAgain_);
    if (hasPlayAgain_) {
        if (!playAgain_) throw pending_reverse("BattleEndMessage needs PlayAgainStatus");
        playAgain_->encode(stream());
    }
    stream().writeBoolean(hasQuests_);
    if (hasQuests_) {
        if (!quests_) throw pending_reverse("BattleEndMessage needs LogicQuests");
        quests_->encode(stream());
    }
    stream().writeVInt(v304_);
    stream().writeVInt(v308_);
    stream().writeBoolean(hasRankedState_);
    if (hasRankedState_) {
        if (!rankedState_) throw pending_reverse("BattleEndMessage needs LogicRankedMatchRoundState");
        rankedState_->encode(stream());
    }
    stream().writeVInt(v312_);
    stream().writeBoolean(hasChronos_);
    if (hasChronos_) {
        if (!chronos_) throw pending_reverse("BattleEndMessage needs ChronosTextEntry");
        chronos_->encode(stream());
    }
}
void BattleEndMessage::decode() {
    PiranhaMessage::decode();
    battleIdA_ = stream().readLongLong();
    battleIdB_ = stream().readLongLong();
    for (i32& x : v11_) x = stream().readVInt();
    hasGemOffer_ = stream().readBoolean();
    if (hasGemOffer_) {
        gemOffer_ = std::make_unique<LogicGemOffer>();
        gemOffer_->decode(stream());
    }
    v196_ = stream().readVInt();
    v200_ = stream().readVInt();
    b204_ = stream().readBoolean();
    v216_ = stream().readVInt();
    v220_ = stream().readVInt();
    for (bool& f : flags_) f = stream().readBoolean();
    v232_ = stream().readVInt();
    b236_ = stream().readBoolean();
    decodeEntryArray(stream(), players_, [] { return std::make_unique<PlayerEntry>(); });
    decodeEntryArray(stream(), xp_, [] { return std::make_unique<XpEntry>(); });
    const i32 nd = stream().readVInt();
    dataRefs_.clear();
    for (i32 i = 0; i < nd; ++i) {
        DataReference d;
        d.classId = stream().readVInt();
        d.instanceId = stream().readVInt();
        dataRefs_.push_back(d);
    }
    decodeEntryArray(stream(), milestones_,
                     [] { return std::make_unique<LogicMilestoneProgress>(); });
    dataRef272_ = DataReference::decodeNullable(stream());
    hasPlayAgain_ = stream().readBoolean();
    if (hasPlayAgain_) {
        playAgain_ = std::make_unique<PlayAgainStatus>();
        playAgain_->decode(stream());
    }
    hasQuests_ = stream().readBoolean();
    if (hasQuests_) {
        quests_ = std::make_unique<LogicQuests>();
        quests_->decode(stream());
    }
    v304_ = stream().readVInt();
    v308_ = stream().readVInt();
    hasRankedState_ = stream().readBoolean();
    if (hasRankedState_) {
        rankedState_ = std::make_unique<LogicRankedMatchRoundState>();
        rankedState_->decode(stream());
    }
    v312_ = stream().readVInt();
    hasChronos_ = stream().readBoolean();
    if (hasChronos_) {
        chronos_ = std::make_unique<ChronosTextEntry>();
        chronos_->decode(stream());
    }
}

// ---- 23458 BattleLogMessage ----
void BattleLogMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(b132_);
    encodeEntryArray(stream(), entries_);
}
void BattleLogMessage::decode() {
    PiranhaMessage::decode();
    b132_ = stream().readBoolean();
    decodeEntryArray(stream(), entries_, [] { return std::make_unique<BattleLogEntry>(); });
}

// ---- 24413 BattleReportStreamMessage ----
void BattleReportStreamMessage::encode() {
    PiranhaMessage::encode();
    ownerId_.encode(stream());
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
void BattleReportStreamMessage::decode() {
    PiranhaMessage::decode();
    ownerId_ = LogicLong::decode(stream());
    const i32 n = stream().readVInt();
    entries_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        const i32 type = stream().readVInt();
        auto e = std::make_unique<BattleReportStreamEntry>();
        e->decode(stream());
        entries_.emplace_back(type, std::move(e));
    }
}

// ---- 10160 BillingCancelledByClientMessage ----
void BillingCancelledByClientMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(productIds_.size()));
    for (const auto& p : productIds_) stream().writeString(&p);
}
void BillingCancelledByClientMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    productIds_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) productIds_.push_back(stream().readString().value_or(""));
}

// ---- Bind* family ----
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

// ---- 24700 BrawlTvChannelListMessage ----
void BrawlTvChannelListMessage::encode() {
    PiranhaMessage::encode();
    encodeEntryArray(stream(), channels_);
}
void BrawlTvChannelListMessage::decode() {
    PiranhaMessage::decode();
    decodeEntryArray(stream(), channels_,
                     [] { return std::make_unique<BrawlTvChannelInfo>(); });
}

} // namespace titan
