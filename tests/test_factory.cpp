// Factory + framing self-check.

#include "titan/gen/MessageFactory.hpp"
#include "titan/net/Messaging.hpp"

#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    // Factory creates by id; created message reports the same id.
    for (int id : {10101, 10116, 10108, 20108, 24101, 20104, 10100, 14103, 20559}) {
        auto m = createMessageByType(id);
        CHECK(m != nullptr);
        if (m) CHECK(m->getMessageType() == id);
    }
    CHECK(createMessageByType(999999) == nullptr);
    // Collision ids still resolve (first alphabetical, documented).
    CHECK(createMessageByType(20101) != nullptr);

    // Framing: header layout [u16 type][u24 len][u16 ver].
    {
        auto m = createMessageByType(10108); // KeepAlive, empty payload
        u8 hdr[net::kHeaderSize];
        m->encode();
        net::writeHeader(*m, hdr, m->getEncodingLength());
        CHECK(hdr[0] == 0x27 && hdr[1] == 0x7C); // 10108 = 0x277C
        CHECK(hdr[2] == 0 && hdr[3] == 0 && hdr[4] == 0);
        CHECK(hdr[5] == 0 && hdr[6] == 0); // version 0
        const net::Header h = net::readHeader(hdr);
        CHECK(h.type == 10108 && h.length == 0 && h.version == 0);
    }
    {
        // Version + length survive the round-trip.
        auto m = createMessageByType(10116);
        m->setMessageVersion(1);
        auto frame = net::encodeFrame(*m); // ResetAccount, 4-byte payload
        CHECK(frame.size() == 7u + 4u);
        const net::Header h = net::readHeader(frame.data());
        CHECK(h.type == 10116 && h.length == 4 && h.version == 1);
    }

    if (failures == 0) std::puts("factory: all ok");
    return failures == 0 ? 0 : 1;
}
