#pragma once

// LogicGiveDeliveryItemsCommand (203) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/messages/pending/DeliveryUnit.hpp"
#include "titan/messages/pending/ForcedDrops.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicGiveDeliveryItemsCommand : public LogicCommand {
public:
    int getCommandType() const override { return 203; }
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    i32 v1_ = 0;
    std::vector<std::unique_ptr<DeliveryUnit>> units_;
    bool hasDrops_ = false;
    std::unique_ptr<ForcedDrops> drops_;
    i32 v2_ = 0, v3_ = 0, v4_ = 0;
    bool b1_ = false, b2_ = false;
};

} // namespace titan
