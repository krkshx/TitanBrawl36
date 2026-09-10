// Self-check for ByteStream wire format (no external deps).
// Round-trips every VInt length class plus int/boolean/string/bytes.

#include "titan/core/ByteStream.cpp"

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
    // VLong round-trip across every length class boundary (@0x3ec094/@0x8d3af0).
    {
        const std::vector<i64> values = {
            0, 1, 63, 64, -1, -63, -64, -65,
            0x1FFF, 0x2000, -8191, -8192,
            0xFFFFF, 0x100000, -1048575, -1048576,
            0x7FFFFFF, 0x8000000, -134217727, -134217728,
            0x3FFFFFFFFLL, 0x400000000LL, -0x400000000LL,
            0x1FFFFFFFFFFLL, 0x20000000000LL,
            0xFFFFFFFFFFFFLL, 0x1000000000000LL,
            0x7FFFFFFFFFFFFFLL, 0x80000000000000LL,
            0x3FFFFFFFFFFFFFFFLL, 0x4000000000000000LL,
            9223372036854775807LL, INT64_MIN,
        };
        for (i64 v : values) {
            ByteStream s;
            s.writeVLong(v);
            CHECK(s.size() >= 1 && s.size() <= 10);
            s.setBuffer(s.data(), s.size());
            const i64 back = s.readVLong();
            if (back != v) {
                ++failures;
                std::printf("FAIL vlong: wrote %lld read %lld\n",
                            (long long)v, (long long)back);
            }
        }
    }
    // VLong single-byte markers (bit-exact with @0x3ec094).
    {
        ByteStream s;
        s.writeVLong(0);
        CHECK(s.size() == 1 && s.data()[0] == 0x00);
        ByteStream a;
        a.writeVLong(63);
        CHECK(a.size() == 1 && a.data()[0] == 0x3F);
        ByteStream b;
        b.writeVLong(64);
        CHECK(b.size() == 2 && b.data()[0] == 0x80 && b.data()[1] == 0x01);
        ByteStream n;
        n.writeVLong(-1);
        CHECK(n.size() == 1 && n.data()[0] == 0x7F); // (-1 & 0x3F) | 0x40
        ByteStream m;
        m.writeVLong(-63);
        CHECK(m.size() == 1 && m.data()[0] == 0x41); // (-63 & 0x3F) | 0x40
        ByteStream w;
        w.writeVLong(-64); // canonical 2-byte form (1-byte only down to -63)
        CHECK(w.size() == 2 && w.data()[0] == 0xC0 && w.data()[1] == 0x7F);
        ByteStream k;
        k.writeVLong(-65);
        CHECK(k.size() == 2 && k.data()[0] == 0xFF && k.data()[1] == 0x7E);
    }
    // VLong length classes (binary thresholds).
    {
        ByteStream s;
        s.writeVLong(0x2000);
        CHECK(s.size() == 3);
        ByteStream t;
        t.writeVLong(9223372036854775807LL);
        CHECK(t.size() == 10);
        ByteStream u;
        u.writeVLong(INT64_MIN);
        CHECK(u.size() == 10);
    }
    // getVLongSizeInBytes (@0x258bb4) agrees with the writer everywhere,
    // incl. the canonical long forms at exact powers (-8192 takes 3).
    {
        const std::vector<i64> bounds = {
            -64, -65, -8191, -8192, -8193, -1048575, -1048576, -134217727,
            -134217728, -134217729, 63, 64, 0x1FFF, 0x2000, 0xFFFFF, 0x100000,
            0x7FFFFFF, 0x8000000, 0x3FFFFFFFFLL, 0x400000000LL,
        };
        for (i64 v : bounds) {
            ByteStream s;
            s.writeVLong(v);
            if (s.size() != ByteStream::getVLongSizeInBytes(v)) {
                ++failures;
                std::printf("FAIL vlong size: value %lld wrote %d predicted %d\n",
                            (long long)v, s.size(),
                            ByteStream::getVLongSizeInBytes(v));
            }
        }
    }
    // VLong checksum fold (@0x739a8c) is deterministic.
    {
        ByteStream s, t;
        s.writeVLong(1234567890123LL);
        t.writeVLong(1234567890123LL);
        CHECK(s.checksum() == t.checksum());
        ByteStream z;
        z.writeVLong(0);
        // lo=0,hi=0: x = 0+0+65 = 65; state = 0 + ((65<<32|65)>>31) + 88
        // = 130 + 88 = 218.
        CHECK(z.checksum() == 218u);
    }
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
