// GetPlayerProfileMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/GetPlayerProfileMessage.hpp"

namespace titan {

void GetPlayerProfileMessage::encode() {
    PiranhaMessage::encode();
    avatarId_.encode(stream());
    stream().writeBoolean(hasEntry_);
    if (hasEntry_) {
        if (!entry_) throw pending_reverse("GetPlayerProfileMessage needs BattleLogPlayerEntry");
        entry_->encode(stream());
    }
    stream().writeVInt(unknown_);
}

void GetPlayerProfileMessage::decode() {
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
