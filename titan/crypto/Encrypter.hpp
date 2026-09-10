#pragma once

// Session encryption — reversed from libg_decrypted.so (ARM64).
//
// PepperEncrypter (size 0x40: key32 @+0x08, nonce24 @+0x28) is NaCl
// secretbox (XSalsa20-Poly1305; the binary vendors tweetnacl:
// crypto_secretbox_tweet @0x9686d8, open @0x577ae8).
//   - MAC overhead 16 bytes (SECRETBOX_MACBYTES @0xb9ff58 = 0x10).
//   - Nonce starts random (generateRandom @0x46a64c), then LE += 2 per
//     message via nextNonce @0x6fb2ac, before each encrypt/decrypt.
//   - Separate instances: Messaging+0x148 (decrypt), +0x150 (encrypt),
//     both plain PepperEncrypter built by Messaging::Messaging @0x1b4b64
//     (NOT PepperPerMessageEncrypter — that class is for another path).
//   - C2(key, nonce) @0x7020c4; encrypt @0x4491dc / decrypt @0x39f6b4
//     (both nextNonce-first, overhead slot = 16).
//   - Key exchange: NaCl box + Blake2b (PepperCrypto::box_open @0x5863c0);
//     PepperLoginResponse (20103) carries 24B server nonce + 32B session
//     key; installed as PepperEncrypter(key, nonce) x2
//     (decrypt nonce = server nonce, encrypt nonce = client nonce).
//   - Messages 10100/10101 bypass encryption (plaintext memcpy).
//   - On decrypt failure the binary sends CryptoError.
//
// Port: libsodium's crypto_secretbox_* is wire-compatible with tweetnacl.
// libsodium is an OPTIONAL dependency (TITAN_WITH_SODIUM); without it the
// factory throws with a clear message so the default build stays green.

#include "titan/core/Types.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

namespace titan::crypto {

constexpr std::size_t kKeyBytes = 32;
constexpr std::size_t kNonceBytes = 24;
constexpr std::size_t kMacBytes = 16;

struct SodiumMissing : std::runtime_error {
    SodiumMissing()
        : std::runtime_error("TitanBrawl36: built without libsodium "
                             "(cmake -DTITAN_WITH_SODIUM=ON)") {}
};

class Encrypter {
public:
    virtual ~Encrypter() = default;
    // Encrypt: returns ciphertext (plaintext + 16B MAC). The nonce
    // advances by 2 BEFORE each use (encrypt @0x4491dc, decrypt @0x39f6b4),
    // so the installed initial value never goes on the wire.
    virtual std::vector<u8> encrypt(const u8* plain, std::size_t len) = 0;
    // Decrypt: returns plaintext. Throws on MAC failure.
    virtual std::vector<u8> decrypt(const u8* cipher, std::size_t len) = 0;
    [[nodiscard]] virtual std::size_t overhead() const { return kMacBytes; }
};

// nextNonce @0x6fb2ac: 24-byte LE counter += 2.
inline void nextNonce(u8 nonce[kNonceBytes]) {
    unsigned carry = 2;
    for (std::size_t i = 0; i < kNonceBytes && carry != 0; ++i) {
        const unsigned v = static_cast<unsigned>(nonce[i]) + carry;
        nonce[i] = static_cast<u8>(v & 0xFF);
        carry = v >> 8;
    }
}

// Builds the session encrypter (libsodium backend). Throws SodiumMissing
// when TITAN_WITH_SODIUM is off.
std::unique_ptr<Encrypter> makePepperEncrypter(const u8 key[kKeyBytes],
                                              const u8 nonce[kNonceBytes]);

} // namespace titan::crypto
