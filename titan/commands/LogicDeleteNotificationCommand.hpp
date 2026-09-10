#pragma once

// LogicDeleteNotificationCommand (514) — split out; wire format unchanged.
// execute @0x961cc4: removeNotification(home, v1_); if the removed entry
// is an IAPDelivery (type 86) its billing package is reported (skipped —
// our IAPDeliveryNotification carries no package field yet). Missing entry
// -> Debugger::warning. Listener callbacks skipped (platform).

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"
#include "titan/sc/Debugger.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicDeleteNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 514; }
    int execute(LogicHomeMode* home, int a, bool b) override {
        (void)a;
        (void)b;
        LogicClientHome* h = home ? home->getHome() : nullptr;
        if (!h) return 0;
        auto removed = h->removeNotification(v1_);
        if (removed) {
            // Change-listener + billing-package reports skipped (platform).
            return 0;
        }
        sc::Debugger::warning("LogicDeleteNotificationCommand: no notification "
                              + std::to_string(v1_));
        return 0;
    }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

} // namespace titan
