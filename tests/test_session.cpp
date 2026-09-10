// End-to-end session loopback: encode -> frame -> parse header ->
// factory-create -> decode. Exercises the whole client path
// (factory + framing + optional session crypto) over real bytes.

#include "titan/crypto/Encrypter.cpp"
#include "titan/gen/MessageFactory.cpp"
#include "titan/messages/account/KeepAliveMessage.cpp"
#include "titan/messages/account/ResetAccountMessage.cpp"
#include "titan/net/Messaging.cpp"

#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

// Loopback channel: byte frames in, parsed messages out.
struct Loopback {
    // Client side: frame an already-filled message.
    static std::vector<u8> send(PiranhaMessage& m, crypto::Encrypter* enc) {
        return net::encodeSendFrame(m, enc);
    }
    // Server side: parse header, build by type, decode payload.
    static std::unique_ptr<PiranhaMessage> receive(const std::vector<u8>& frame,
                                                   crypto::Encrypter* enc) {
        if (frame.size() < static_cast<std::size_t>(net::kHeaderSize)) return nullptr;
        u8 hdr[net::kHeaderSize];
        for (int i = 0; i < net::kHeaderSize; ++i) hdr[i] = frame[i];
        const net::Header h = net::readHeader(hdr);
        if (h.length < 0
            || static_cast<std::size_t>(h.length) != frame.size() - net::kHeaderSize) {
            return nullptr;
        }
        auto m = createMessageByType(h.type);
        if (!m) return nullptr;
        m->setMessageVersion(h.version);
        std::vector<u8> body(frame.begin() + net::kHeaderSize, frame.end());
        if (enc != nullptr && h.type != 10100 && h.type != 10101) {
            body = enc->decrypt(body.data(), body.size());
        }
        // NB: setBuffer rejects null even at length 0; empty payloads
        // decode straight from the fresh stream.
        if (!body.empty()) {
            m->stream().setBuffer(body.data(), static_cast<i32>(body.size()));
        }
        m->decode();
        return m;
    }
};

// Test encrypter: reversible tag transform (NOT the real secretbox;
// crypto-secretbox vectors live in test_crypto).
struct TagEncrypter : crypto::Encrypter {
    std::vector<u8> encrypt(const u8* plain, std::size_t len) override {
        std::vector<u8> out(plain, plain + len);
        for (auto& b : out) b ^= 0x5A;
        out.insert(out.end(), crypto::kMacBytes, 0xEE);
        return out;
    }
    std::vector<u8> decrypt(const u8* cipher, std::size_t len) override {
        if (len < crypto::kMacBytes) throw std::runtime_error("short");
        std::vector<u8> out(cipher, cipher + len - crypto::kMacBytes);
        for (auto& b : out) b ^= 0x5A;
        return out;
    }
};

int main() {
    // 1. Plaintext leg: KeepAlive out and back.
    {
        KeepAliveMessage out;
        const auto frame = Loopback::send(out, nullptr);
        CHECK(frame.size() == 7u);
        auto back = Loopback::receive(frame, nullptr);
        CHECK(back && back->getMessageType() == 10108);
    }
    // 2. Payload survives framing (ResetAccount preset = 3).
    {
        ResetAccountMessage out;
        out.setAccountPreset(3);
        const auto frame = Loopback::send(out, nullptr);
        CHECK(frame.size() == 7u + 4u);
        auto back = Loopback::receive(frame, nullptr);
        CHECK(back && back->getMessageType() == 10116);
        const auto* ra = static_cast<const ResetAccountMessage*>(back.get());
        CHECK(ra->accountPreset() == 3);
    }
    // 3. Version rides the header.
    {
        ResetAccountMessage out;
        out.setMessageVersion(2);
        const auto frame = Loopback::send(out, nullptr);
        auto back = Loopback::receive(frame, nullptr);
        CHECK(back && back->getMessageVersion() == 2);
    }
    // 4. Encrypted leg: same payload through the session transform.
    {
        TagEncrypter enc;
        ResetAccountMessage out;
        out.setAccountPreset(7);
        const auto frame = Loopback::send(out, &enc);
        CHECK(frame.size() == 7u + 4u + crypto::kMacBytes);
        auto back = Loopback::receive(frame, &enc);
        CHECK(back && back->getMessageType() == 10116);
        const auto* ra = static_cast<const ResetAccountMessage*>(back.get());
        CHECK(ra->accountPreset() == 7);
    }
    // 5. Unknown ids and truncated frames are rejected, not crashed on.
    {
        KeepAliveMessage out;
        auto frame = Loopback::send(out, nullptr);
        frame[0] = 0xFF;
        frame[1] = 0xFF; // type 65535: no such message
        CHECK(Loopback::receive(frame, nullptr) == nullptr);
        frame.resize(3); // truncated header
        CHECK(Loopback::receive(frame, nullptr) == nullptr);
    }

    if (failures == 0) std::puts("session: all ok");
    return failures == 0 ? 0 : 1;
}
