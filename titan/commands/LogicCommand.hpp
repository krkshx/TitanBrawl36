#pragma once

// LogicCommand — base of every game command, reversed from libg_decrypted.so.
//
// Dispatch (LogicCommandManager): encodeCommand @0x870160 writes
// writeVInt(getCommandType) then the virtual body; decodeCommand @0x2b856c
// reads the vint, createCommand @0x7c41d8 builds it, verifies the type,
// then calls the virtual decode. Manager encode @0x85e668 / decode @0x38a110
// wrap a vint count around per-command (type + body) pairs.
//
// Base body: encode @0x50f3e4 = writeVInt(+16), writeVInt(+12),
// LogicLong(+20); decode @0x513270 mirrors. Ctor @0x49c8c4 defaults:
// flag=0, +12=-1, +16=-1, long 0/0.
//
// Gameplay semantics: execute(home, a, b) runs the command against
// LogicHomeMode (e.g. DiamondsAdded @0x44d194). The base default is a
// no-op; each command overrides it in its own wave. GameListener UI
// callbacks inside execute() are platform code and skipped explicitly.
//
// Manager-level helpers live in titan/gen/LogicCommands.hpp (generated).

#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicHomeMode.hpp"
#include "titan/messages/Nested.cpp"

namespace titan {

class LogicCommand : public NestedEntry {
public:
    LogicCommand() = default;
    ~LogicCommand() override = default;

    virtual int getCommandType() const = 0;
    virtual int execute(LogicHomeMode* home, int a, bool b) {
        (void)home;
        (void)a;
        (void)b;
        return 0;
    }

    void encode(ByteStream& s) const override {
        s.writeVInt(v16_);
        s.writeVInt(v12_);
        id_.encode(s);
    }
    void decode(ByteStream& s) override {
        v16_ = s.readVInt();
        v12_ = s.readVInt();
        id_ = LogicLong::decode(s);
    }

    i32 v16_ = -1, v12_ = -1;
    LogicLong id_;
    bool flag_ = false; // +8
};

} // namespace titan
