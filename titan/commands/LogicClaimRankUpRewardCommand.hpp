#pragma once

// LogicClaimRankUpRewardCommand (517) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicClaimRankUpRewardCommand : public LogicCommand {
public:
    int getCommandType() const override { return 517; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v2_);
        s.writeVInt(v3_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
        v2_ = s.readVInt();
        v3_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
    std::optional<DataReference> ref_;
};

} // namespace titan
