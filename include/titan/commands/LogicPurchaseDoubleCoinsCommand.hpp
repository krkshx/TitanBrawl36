#pragma once

// LogicPurchaseDoubleCoinsCommand (509) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicPurchaseDoubleCoinsCommand : public LogicCommand {
public:
    int getCommandType() const override { return 509; }
};

} // namespace titan
