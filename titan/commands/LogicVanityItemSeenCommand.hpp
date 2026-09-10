#pragma once

// LogicVanityItemSeenCommand (537) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicVanityItemSeenCommand : public LogicCommand {
public:
    int getCommandType() const override { return 537; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        DataReference::encodeNullable(s, ref_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        ref_ = DataReference::decodeNullable(s);
    }
    std::optional<DataReference> ref_;
};

} // namespace titan
