#pragma once

// GetPlayerProfileMessage (14113) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/battle/BattleLogPlayerEntry.cpp"

namespace titan {

class GetPlayerProfileMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14113; }
    const char* getMessageTypeName() const override { return "GetPlayerProfileMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    bool hasEntry_ = false;
    std::unique_ptr<BattleLogPlayerEntry> entry_;
    i32 unknown_ = 0;
};

} // namespace titan

// GetPlayerProfileMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void GetPlayerProfileMessage::encode() {
PiranhaMessage::encode();
    avatarId_.encode(stream());
    stream().writeBoolean(hasEntry_);
    if (hasEntry_) {
        if (!entry_) throw pending_reverse("GetPlayerProfileMessage needs BattleLogPlayerEntry");
        entry_->encode(stream());
    }
    stream().writeVInt(unknown_);
}

inline void GetPlayerProfileMessage::decode() {
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
