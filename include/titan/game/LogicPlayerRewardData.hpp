#pragma once

// LogicPlayerRewardData::encode @0x243790.
// Wire: bool + LogicRewardConfig @+8 (nullable), vint @+16, bool @+20.
// Decode is default-ctor + decode (see LogicPlayerRankedSeasonData::decode
// @0x97acd8): same reads in the same order (mirrored below).

#include "titan/messages/Nested.hpp"
#include "titan/game/LogicRewardConfig.hpp"

#include <memory>

namespace titan {

class LogicPlayerRewardData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeBoolean(config_ != nullptr);
        if (config_) config_->encode(s);
        s.writeVInt(v16_);
        s.writeBoolean(b20_);
    }
    void decode(ByteStream& s) override {
        if (s.readBoolean()) {
            config_ = std::make_unique<LogicRewardConfig>();
            config_->decode(s);
        } else {
            config_.reset();
        }
        v16_ = s.readVInt();
        b20_ = s.readBoolean();
    }

    std::unique_ptr<LogicRewardConfig> config_; // +8 (nullable)
    i32 v16_ = 0;                               // +16
    bool b20_ = false;                          // +20
};

} // namespace titan
