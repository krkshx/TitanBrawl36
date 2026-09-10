#pragma once

// LogicSelectSkinCommand (506) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicSelectSkinCommand : public LogicCommand {
public:
    int getCommandType() const override { return 506; }
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
