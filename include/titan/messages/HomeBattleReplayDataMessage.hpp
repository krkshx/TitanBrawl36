#pragma once

// HomeBattleReplayDataMessage (24114) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class HomeBattleReplayDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24114; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayDataMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(replaySlot_);
        if (replayData_) {
            const auto& p = *replayData_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        replaySlot_ = stream().readVInt();
        replayData_ = stream().readBytesNullable();
    }
    i32 replaySlot_ = 0;
    std::optional<std::vector<u8>> replayData_;
};

} // namespace titan
