#pragma once

// LogicPurchaseChallengeLivesCommand (540) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicPurchaseChallengeLivesCommand : public LogicCommand {
public:
    int getCommandType() const override { return 540; }
};

} // namespace titan
