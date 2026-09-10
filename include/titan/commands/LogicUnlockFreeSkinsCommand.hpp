#pragma once

// LogicUnlockFreeSkinsCommand (526) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicUnlockFreeSkinsCommand : public LogicCommand {
public:
    int getCommandType() const override { return 526; }
};

} // namespace titan
