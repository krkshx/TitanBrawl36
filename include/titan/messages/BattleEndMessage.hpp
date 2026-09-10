#pragma once

// BattleEndMessage (23456) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/ChronosTextEntry.hpp"
#include "titan/messages/pending/LogicGemOffer.hpp"
#include "titan/messages/pending/LogicMilestoneProgress.hpp"
#include "titan/messages/pending/LogicQuests.hpp"
#include "titan/messages/pending/LogicRankedMatchRoundState.hpp"
#include "titan/messages/pending/PlayAgainStatus.hpp"
#include "titan/messages/pending/PlayerEntry.hpp"
#include "titan/messages/pending/XpEntry.hpp"

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
