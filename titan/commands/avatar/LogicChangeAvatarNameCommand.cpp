#pragma once

// LogicChangeAvatarNameCommand (201) — split out; wire format unchanged.
// execute @0x1b40d4: empty name -> 1, no avatar -> 2, else charge
// useDiamonds(name length), set name + nameSetByUser. Listener callbacks
// skipped (platform).

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicChangeAvatarNameCommand : public LogicCommand {
public:
    int getCommandType() const override { return 201; }
    int execute(LogicHomeMode* home, int a, bool b) override {
        (void)a;
        (void)b;
        if (name_.empty()) return 1;
        LogicClientAvatar* avatar = home ? home->getPlayerAvatar() : nullptr;
        if (!avatar) return 2;
        if (!name_.empty()) {
            // Cost unit is the binary String length (bytes for ASCII).
            avatar->useDiamonds(static_cast<i32>(name_.size()));
            // Avatar listener callback skipped (platform).
        }
        const bool wasSet = avatar->getNameSetByUser();
        (void)wasSet; // reported to the GameListener in the binary
        avatar->setName(name_);
        avatar->setNameSetByUser(true);
        // GameListener + avatar listener callbacks skipped (platform).
        return 0;
    }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeStringReference(name_);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        name_ = s.readStringReference();
        v_ = s.readVInt();
    }
    std::string name_;
    i32 v_ = 0;
};

} // namespace titan
