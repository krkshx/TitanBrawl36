#pragma once

// LogicDiamondsAddedCommand (202) — split out; wire format unchanged.
// execute @0x44d194: adds i1_ to diamonds; free-diamonds or purchase
// bookkeeping by b_. GameListener UI callbacks skipped (platform).

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicDiamondsAddedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 202; }
    int execute(LogicHomeMode* home, int a, bool b) override {
        (void)a;
        (void)b;
        LogicClientAvatar* avatar = home ? home->getPlayerAvatar() : nullptr;
        if (!avatar) return -1;
        avatar->setDiamonds(i1_ + avatar->getDiamonds());
        if (b_) {
            if (i1_ >= 0) {
                avatar->setFreeDiamonds(i1_ + avatar->getFreeDiamonds());
            } else if (avatar->getFreeDiamonds() > avatar->getDiamonds()) {
                avatar->setFreeDiamonds(avatar->getDiamonds());
            }
            return 0;
        }
        // GameListener::diamondsAddedToInbox + refresh skipped (platform).
        avatar->addCumulativePurchasedDiamonds(i1_);
        return 0;
    }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(b_);
        s.writeInt(i1_);
        s.writeInt(i2_);
        s.writeInt(i3_);
        s.writeString(str_ ? &*str_ : nullptr);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        b_ = s.readBoolean();
        i1_ = s.readInt();
        i2_ = s.readInt();
        i3_ = s.readInt();
        str_ = s.readString();
    }
    bool b_ = false;
    i32 i1_ = 0, i2_ = 0, i3_ = 0;
    std::optional<std::string> str_;
};

} // namespace titan
