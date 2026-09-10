#pragma once

// LogicToggleInGameHintsCommand (512) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicToggleInGameHintsCommand : public LogicCommand {
public:
    int getCommandType() const override { return 512; }
};

} // namespace titan
