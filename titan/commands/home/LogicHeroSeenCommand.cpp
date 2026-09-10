#pragma once

// LogicHeroSeenCommand (522) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicHeroSeenCommand : public LogicCommand {
public:
    int getCommandType() const override { return 522; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        DataReference::encodeNullable(s, ref_);
        s.writeInt(i_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        ref_ = DataReference::decodeNullable(s);
        i_ = s.readInt();
    }
    std::optional<DataReference> ref_;
    i32 i_ = 0;
};

} // namespace titan
