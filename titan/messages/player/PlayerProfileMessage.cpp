#pragma once

// PlayerProfileMessage (24113) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
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

// PlayerProfileMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.


namespace titan {

inline void PlayerProfileMessage::encode() {
PiranhaMessage::encode();
    if (!profile_) throw pending_reverse("PlayerProfileMessage needs PlayerProfile");
    profile_->encode(stream());
    stream().writeBoolean(hasAlliance_);
    if (hasAlliance_) {
        if (!alliance_) throw pending_reverse("PlayerProfileMessage needs AllianceHeaderEntry");
        alliance_->encode(stream());
    }
DataReference::encodeNullable(stream(), ref152_);
}

inline void PlayerProfileMessage::decode() {
PiranhaMessage::decode();
    profile_ = std::make_unique<PlayerProfile>();
    profile_->decode(stream());
    hasAlliance_ = stream().readBoolean();
    if (hasAlliance_) {
        alliance_ = std::make_unique<AllianceHeaderEntry>();
        alliance_->decode(stream());
    }
    ref152_ = DataReference::decodeNullable(stream());
}


} // namespace titan
