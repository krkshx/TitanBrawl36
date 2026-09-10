#pragma once

// LogicOffersChangedCommand (211) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/game/shop/LogicOfferBundle.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicOffersChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 211; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(isNull_ ? 0 : static_cast<i32>(bundles_.size()));
        for (const auto& b : bundles_) b->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        const i32 n = s.readVInt();
        bundles_.clear();
        isNull_ = (n == 0); // 0 is ambiguous (empty or null); assume null
        for (i32 i = 0; i < n; ++i) {
            auto b = std::make_unique<LogicOfferBundle>();
            b->decode(s);
            bundles_.push_back(std::move(b));
        }
    }
    std::vector<std::unique_ptr<LogicOfferBundle>> bundles_;
    bool isNull_ = false;
};

} // namespace titan
