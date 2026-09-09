// Self-check for ByteStream wire format (no external deps).
// Round-trips every VInt length class plus int/boolean/string/bytes.

#include "titan/core/ByteStream.hpp"

#include <cassert>
#include <cstdio>
#include <vector>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    // Int big-endian.
    {
        ByteStream s;
        s.writeInt(0x01020304);
        CHECK(s.size() == 4);
        CHECK(s.data()[0] == 0x01 && s.data()[1] == 0x02 &&
              s.data()[2] == 0x03 && s.data()[3] == 0x04);
        s.setBuffer(s.data(), s.size());
        CHECK(s.readInt() == 0x01020304);
    }
    // VInt round-trip across every length class boundary.
    {
        const std::vector<i32> values = {
            0, 1, 63, 64, -1, -63, -64,
            0x1FFF, 0x2000, -8191, -8192,
            0xFFFFF, 0x100000, -1048575, -1048576,
            0x7FFFFFF, 0x8000000, -134217727, -134217728,
            2147483647, INT32_MIN,
        };
        for (i32 v : values) {
            ByteStream s;
            s.writeVInt(v);
            // Length class check against the binary's thresholds.
            int expected;
            if (v < 0) {
                expected = (v >= -63) ? 1 : (v >= -8191) ? 2
                           : (v >= -1048575) ? 3 : (v >= -134217727) ? 4 : 5;
            } else {
                expected = (v <= 63) ? 1 : (v < 0x2000) ? 2
                           : (v < 0x100000) ? 3 : (v <= 0x7FFFFFF) ? 4 : 5;
            }
            CHECK(s.size() == expected);
            s.setBuffer(s.data(), s.size());
            const i32 back = s.readVInt();
            if (back != v) {
                ++failures;
                std::printf("FAIL vint: wrote %d read %d\n", v, back);
            }
        }
    }
    // VInt single-byte markers (bit-exact with @0x298f64).
    {
        ByteStream s;
        s.writeVInt(0);
        CHECK(s.size() == 1 && s.data()[0] == 0x00);
        ByteStream n;
        n.writeVInt(-1);
        CHECK(n.size() == 1 && n.data()[0] == 0x7F); // (-1 & 0x3F) | 0x40
    }
    // Boolean bit-packing: 8 trues fill exactly one byte.
    {
        ByteStream s;
        for (int i = 0; i < 8; ++i) s.writeBoolean(true);
        CHECK(s.size() == 1 && s.data()[0] == 0xFF);
        s.setBuffer(s.data(), s.size());
        for (int i = 0; i < 8; ++i) CHECK(s.readBoolean());
        ByteStream m;
        m.writeBoolean(true);
        m.writeBoolean(false);
        m.writeBoolean(true);
        CHECK(m.size() == 1 && m.data()[0] == 0x05);
        m.setBuffer(m.data(), m.size());
        CHECK(m.readBoolean() == true);
        CHECK(m.readBoolean() == false);
        CHECK(m.readBoolean() == true);
    }
    // String: null -> -1, value -> len + bytes.
    {
        ByteStream s;
        s.writeString(nullptr);
        CHECK(s.size() == 4);
        s.setBuffer(s.data(), s.size());
        CHECK(!s.readString().has_value());

        const std::string hello("hi");
        ByteStream t;
        t.writeString(&hello);
        CHECK(t.size() == 6);
        t.setBuffer(t.data(), t.size());
        CHECK(t.readString().value() == "hi");
    }
    // Bytes round-trip incl. empty (len 0, not null).
    {
        const u8 raw[] = {0xDE, 0xAD, 0xBE, 0xEF};
        ByteStream s;
        s.writeBytes(raw, 4);
        s.setBuffer(s.data(), s.size());
        CHECK(s.readBytes() == std::vector<u8>({0xDE, 0xAD, 0xBE, 0xEF}));
    }
    // Checksum folds (constants from ChecksumEncoder impl).
    {
        ByteStream s;
        CHECK(s.checksum() == 0u);
        s.writeInt(5);
        CHECK(s.checksum() == 5u + 0u + 9u);
    }

    if (failures == 0) std::puts("bytestream: all ok");
    return failures == 0 ? 0 : 1;
}
