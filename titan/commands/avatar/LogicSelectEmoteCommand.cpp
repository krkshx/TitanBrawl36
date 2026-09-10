#pragma once

// LogicSelectEmoteCommand (538) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicSelectEmoteCommand : public LogicCommand {
public:
    int getCommandType() const override { return 538; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        ref_ = DataReference::decodeNullable(s);
        v_ = s.readVInt();
    }
    std::optional<DataReference> ref_;
    i32 v_ = 0;
};

} // namespace titan
