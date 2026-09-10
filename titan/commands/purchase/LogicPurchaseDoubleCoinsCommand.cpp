#pragma once

// LogicPurchaseDoubleCoinsCommand (509) — split out; wire format unchanged.
// execute @0x92b034: if affordable (diamonds >= conf 10006), charge via
// useDiamonds and add conf 10007 to daily tail43_[0]. Listener callbacks
// skipped (platform). Null daily_ returns safely (binary would crash).

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicPurchaseDoubleCoinsCommand : public LogicCommand {
public:
    int getCommandType() const override { return 509; }
    int execute(LogicHomeMode* home, int a, bool b) override {
        (void)a;
        (void)b;
        if (!home) return 0;
        LogicClientAvatar* avatar = home->getPlayerAvatar();
        if (!avatar) return 0;
        LogicClientHome* h = home->getHome();
        if (!h || !h->conf_ || !h->daily_) return 0;
        const i32 price = h->conf_->getIntValue(10006);
        if (avatar->getDiamonds() < price) return 0;
        avatar->useDiamonds(price);
        h->daily_->tail43_[0] += h->conf_->getIntValue(10007);
        return 0;
    }
};

} // namespace titan
