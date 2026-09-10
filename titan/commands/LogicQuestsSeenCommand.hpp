#pragma once

// LogicQuestsSeenCommand (533) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicQuestsSeenCommand : public LogicCommand {
public:
    int getCommandType() const override { return 533; }
};

} // namespace titan
