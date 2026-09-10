// BattleLogMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.

#include "titan/messages/BattleLogMessage.hpp"

namespace titan {

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


} // namespace titan
