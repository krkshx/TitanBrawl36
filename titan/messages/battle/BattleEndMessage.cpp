#pragma once

// BattleEndMessage (23456) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/ChronosTextEntry.hpp"
#include "titan/game/LogicGemOffer.hpp"
#include "titan/game/LogicMilestoneProgress.hpp"
#include "titan/game/LogicQuests.hpp"
#include "titan/game/LogicRankedMatchRoundState.hpp"
#include "titan/game/PlayAgainStatus.hpp"
#include "titan/game/PlayerEntry.hpp"
#include "titan/game/XpEntry.hpp"

namespace titan {

class BattleEndMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23456; }
    const char* getMessageTypeName() const override { return "BattleEndMessage"; }
    void encode() override;
    void decode() override;
    i64 battleIdA_ = 0, battleIdB_ = 0;
    i32 v11_[11] = {}; // v152..v192 in wire order
    bool hasGemOffer_ = false;
    std::unique_ptr<LogicGemOffer> gemOffer_;
    i32 v196_ = 0, v200_ = 0;
    bool b204_ = false;
    i32 v216_ = 0, v220_ = 0;
    bool flags_[6] = {}; // b224..b229
    i32 v232_ = 0;
    bool b236_ = false;
    std::vector<std::unique_ptr<PlayerEntry>> players_;
    std::vector<std::unique_ptr<XpEntry>> xp_;
    std::vector<DataReference> dataRefs_;
    std::vector<std::unique_ptr<LogicMilestoneProgress>> milestones_;
    std::optional<DataReference> dataRef272_;
    bool hasPlayAgain_ = false;
    std::unique_ptr<PlayAgainStatus> playAgain_;
    bool hasQuests_ = false;
    std::unique_ptr<LogicQuests> quests_;
    i32 v304_ = 0, v308_ = 0;
    bool hasRankedState_ = false;
    std::unique_ptr<LogicRankedMatchRoundState> rankedState_;
    i32 v312_ = 0;
    bool hasChronos_ = false;
    std::unique_ptr<ChronosTextEntry> chronos_;
};

} // namespace titan

// BattleEndMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.


namespace titan {

inline void BattleEndMessage::encode() {
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

inline void BattleEndMessage::decode() {
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


} // namespace titan
