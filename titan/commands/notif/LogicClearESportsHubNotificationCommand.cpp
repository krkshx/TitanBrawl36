#pragma once

// LogicClearESportsHubNotificationCommand (541) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicClearESportsHubNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 541; }
};

} // namespace titan
