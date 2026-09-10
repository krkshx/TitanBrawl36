#pragma once

// LogicPurchaseOfferCommand (519) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicPurchaseOfferCommand : public LogicCommand {
public:
    int getCommandType() const override { return 519; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        DataReference::encodeNullable(s, ref_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
    }
    i32 v_ = 0;
    std::optional<DataReference> ref_;
};

} // namespace titan
