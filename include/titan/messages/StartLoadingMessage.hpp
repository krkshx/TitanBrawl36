#pragma once

// StartLoadingMessage (20559) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/BattlePlayerMap.hpp"
#include "titan/messages/pending/LogicPlayer.hpp"
#include "titan/messages/pending/LogicVector2.hpp"

namespace titan {

class StartLoadingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20559; }
    const char* getMessageTypeName() const override { return "StartLoadingMessage"; }
    void encode() override;
    void decode() override;
    i32 head_[3] = {}; // unk132..140
    std::vector<std::unique_ptr<LogicPlayer>> players_;   // int count
    std::vector<std::unique_ptr<LogicVector2>> spawns_;   // int count
    std::vector<i32> unkInts_;                            // int count
    i32 unk144_ = 0;
    i32 vints_[5] = {}; // unk148..164 (vint)
    // NOTE: spec order lists vint unk168 between unk164 and mapRef.
    i32 v168_ = 0;
    std::optional<DataReference> mapRef_;
    std::unique_ptr<BattlePlayerMap> battleMap_;
    bool unk172_ = false, unk173_ = false;
    i32 v176_ = 0, v180_ = 0;
};

} // namespace titan
