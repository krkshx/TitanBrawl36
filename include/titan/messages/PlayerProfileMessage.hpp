#pragma once

// PlayerProfileMessage (24113) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/PlayerProfile.hpp"

namespace titan {

class PlayerProfileMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24113; }
    const char* getMessageTypeName() const override { return "PlayerProfileMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<PlayerProfile> profile_;
    bool hasAlliance_ = false;
    std::unique_ptr<AllianceHeaderEntry> alliance_;
    std::optional<DataReference> ref152_;
};

} // namespace titan
