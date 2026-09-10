#pragma once

// LogicClientHome::encode @0x485314, decode @0x3b1044.
// Wire: LogicDailyData, LogicConfData, logiclong, vint count +
// (vint type + Notification) loop, vint, bool, optional GatchaDrop vector,
// vint count + dataref loop. Nested data classes stay pending.
// Split out of the entry wave; wire format unchanged.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/GatchaDrop.hpp"
#include "titan/game/LogicConfData.hpp"
#include "titan/game/LogicDailyData.hpp"
#include "titan/messages/pending/Notification.cpp"

#include <memory>
#include <vector>

namespace titan {

class LogicClientHome : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    // removeNotification @0x8f73e4: removes the first entry whose base
    // +8 int (v8_) equals id, transferring ownership (null if absent).
    std::unique_ptr<Notification> removeNotification(i32 id);
    std::unique_ptr<LogicDailyData> daily_;
    std::unique_ptr<LogicConfData> conf_;
    LogicLong id_;
    std::vector<std::pair<i32, std::unique_ptr<Notification>>> notifications_;
    i32 v76_ = 0;
    bool b72_ = false;
    std::vector<std::unique_ptr<GatchaDrop>> drops_;
    bool dropsNull_ = false; // null -> vint 0 on the wire
    std::vector<DataReference> refs_;
};

} // namespace titan
