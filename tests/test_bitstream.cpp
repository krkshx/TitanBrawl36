// Self-check for BitStream (LSB-first) and LogicCommand dispatch.

#include "titan/core/BitStream.hpp"
#include "titan/gen/LogicCommands.hpp"

#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    // writePositiveInt(5, 3) -> bits 101 LSB-first in one byte = 0b101.
    {
        BitStream s;
        s.writePositiveInt(5, 3);
        CHECK(s.getByteArray()[0] == 0x05);
        BitStream d;
        d.setBuffer(s.getByteArray(), s.getLength());
        CHECK(d.readPositiveInt(3) == 5);
    }
    // Booleans pack LSB-first: true,false,true -> 0b101.
    {
        BitStream s;
        s.writeBoolean(true);
        s.writeBoolean(false);
        s.writeBoolean(true);
        CHECK(s.getByteArray()[0] == 0x05);
        BitStream d;
        d.setBuffer(s.getByteArray(), s.getLength());
        CHECK(d.readBoolean() && !d.readBoolean() && d.readBoolean());
    }
    // writeInt sign-magnitude: -3 in 2 bits -> sign 0, mag 11 -> 0b110.
    {
        BitStream s;
        s.writeInt(-3, 2);
        CHECK(s.getByteArray()[0] == 0x06);
    }
    // writeInt(3, 2) -> sign 1, mag 11 -> 0b111.
    {
        BitStream s;
        s.writeInt(3, 2);
        CHECK(s.getByteArray()[0] == 0x07);
    }
    // Clamping: writePositiveInt(999, 10) clamps to 1023.
    {
        BitStream s;
        s.writePositiveInt(9999, 10);
        BitStream d;
        d.setBuffer(s.getByteArray(), s.getLength());
        CHECK(d.readPositiveInt(10) == 1023);
    }
    // ClientInput-style sequence: 14/10/13/10/10-bit positives.
    {
        BitStream s;
        s.writePositiveInt(16383, 14);
        s.writePositiveInt(1023, 10);
        s.writePositiveInt(8191, 13);
        s.writePositiveInt(1, 10);
        s.writePositiveInt(2, 10);
        BitStream d;
        d.setBuffer(s.getByteArray(), s.getLength());
        CHECK(d.readPositiveInt(14) == 16383);
        CHECK(d.readPositiveInt(10) == 1023);
        CHECK(d.readPositiveInt(13) == 8191);
        CHECK(d.readPositiveInt(10) == 1);
        CHECK(d.readPositiveInt(10) == 2);
    }
    // VInt prefix round-trip.
    {
        BitStream s;
        s.writePositiveVInt(300, 4);
        BitStream d;
        d.setBuffer(s.getByteArray(), s.getLength());
        CHECK(d.readPositiveVInt(4) == 300);
    }
    // Command factory: every table id creates with matching type.
    {
        int made = 0;
        for (int t = 201; t <= 542; ++t) {
            auto c = createCommandByType(t);
            if (c) {
                CHECK(c->getCommandType() == t);
                ++made;
            }
        }
        CHECK(made == 60);
        CHECK(createCommandByType(1000) == nullptr);
        CHECK(createCommandByType(511)->getCommandType() == 511);
    }
    // Command base prefix round-trips (encode @0x50f3e4 shape).
    {
        LogicCommandBase511 c;
        c.v16_ = 7;
        c.v12_ = 8;
        c.id_ = LogicLong{1, 2};
        ByteStream s;
        c.encode(s);
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        LogicCommandBase511 back;
        back.decode(d);
        CHECK(back.v16_ == 7 && back.v12_ == 8 && back.id_.low == 2);
    }
    // Command list helpers: empty list round-trips; unknown type throws.
    {
        ByteStream s;
        std::vector<std::unique_ptr<LogicCommand>> empty;
        encodeCommandList(s, empty);
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        std::vector<std::unique_ptr<LogicCommand>> back;
        decodeCommandList(d, back);
        CHECK(back.empty());
    }

    if (failures == 0) std::puts("bitstream: all ok");
    return failures == 0 ? 0 : 1;
}
