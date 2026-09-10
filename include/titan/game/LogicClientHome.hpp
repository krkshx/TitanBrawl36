#pragma once

// LogicClientHome::encode @0x485314, decode @0x3b1044.
// Wire: LogicDailyData, LogicConfData, logiclong, vint count +
// (vint type + Notification) loop, vint, bool, optional GatchaDrop vector,
// vint count + dataref loop. Nested data classes stay pending.
// Split out of the entry wave; wire format unchanged.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"
#include "titan/messages/pending/GatchaDrop.hpp"
#include "titan/messages/pending/LogicConfData.hpp"
#include "titan/messages/pending/LogicDailyData.hpp"
#include "titan/messages/pending/Notification.hpp"

#include <memory>
#include <vector>

namespace titan {

class LogicClientHome : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
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
