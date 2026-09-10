// Factory + framing self-check.

#include "titan/gen/MessageFactory.cpp"
#include "titan/net/Messaging.cpp"

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
    {
        // Send path @0x93221c: stub encrypter with 16B overhead.
        struct StubEnc : crypto::Encrypter {
            std::vector<u8> encrypt(const u8* plain, std::size_t len) override {
                std::vector<u8> out(plain, plain + len);
                out.insert(out.end(), crypto::kMacBytes, 0xEE);
                return out;
            }
            std::vector<u8> decrypt(const u8* cipher, std::size_t len) override {
                return std::vector<u8>(cipher, cipher + len - crypto::kMacBytes);
            }
        } enc;
        // No session yet -> plaintext, identical to encodeFrame.
        // (NB: encode() appends, so each frame needs a fresh instance.)
        {
            auto a = createMessageByType(10116);
            auto b = createMessageByType(10116);
            CHECK(net::encodeSendFrame(*a, nullptr) == net::encodeFrame(*b));
        }
        // Session active -> body grows by the MAC, header covers it.
        {
            auto m = createMessageByType(10116);
            auto frame = net::encodeSendFrame(*m, &enc);
            CHECK(frame.size() == 7u + 4u + crypto::kMacBytes);
            const net::Header h = net::readHeader(frame.data());
            CHECK(h.type == 10116 && h.length == 4 + 16);
            CHECK(frame[7 + 4] == 0xEE && frame.back() == 0xEE);
        }
        // 10100/10101 bypass encryption even with a session (pre-login).
        {
            auto a = createMessageByType(10100); // ClientHello
            auto b = createMessageByType(10100);
            CHECK(net::encodeSendFrame(*a, &enc) == net::encodeFrame(*b));
        }
    }

    if (failures == 0) std::puts("factory: all ok");
    return failures == 0 ? 0 : 1;
}
