#pragma once

// LogicAddNotificationCommand (206) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicAddNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 206; }
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    bool hasNested_ = false;
    i32 type_ = 0;
    std::unique_ptr<NestedEntry> object_; // concrete class pending (virtual-encode)
};

} // namespace titan
