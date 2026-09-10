#pragma once

// PepperCrypto::box/box_open — NaCl public-key handshake primitives.
//
// Binary (ARM64, libg_decrypted.so):
//   box_open @0x5863c0 (cipher, cipherLen, out, ?, nonce24, key32):
//     buf = 16 zero bytes + cipher; tweet_open(buf, buf, len+16, n, y, x);
//     on success copies buf+32 (len-16 bytes) to out; returns true on
//     FAILURE (inverted vs usual). box @0x3677b0 is the mirror seal.
// This is the standard padded tweetnacl box API (16 zero-byte ciphertext
// prefix = BOXZEROBYTES, 32 zero-byte plaintext prefix = ZEROBYTES),
// wire-compatible with libsodium's crypto_box/crypto_box_open.
//
// Our API returns std::optional (nullopt = auth failure) instead of the
// inverted bool. Requires libsodium (TITAN_WITH_SODIUM); without it the
// calls throw SodiumMissing like the rest of the crypto backend.
// NOTE: which of the two binary key slots (a4/a6) is pk vs sk is resolved
// at the login-handler call site (follow-up); this unit takes them
// explicitly as (peerPublic, ownSecret) per the NaCl convention.

#include "titan/crypto/Encrypter.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <vector>

#ifdef TITAN_WITH_SODIUM
#include <sodium.h>
#endif

namespace titan::crypto {

#ifdef TITAN_WITH_SODIUM
// Seal: cipher = box(plain) (len + 16 MAC bytes).
inline std::vector<u8> pepperBoxSeal(const u8* plain, std::size_t len,
                                     const u8 nonce[kNonceBytes],
                                     const u8 peerPublic[kKeyBytes],
                                     const u8 ownSecret[kKeyBytes]) {
    std::vector<u8> buf(crypto_box_ZEROBYTES + len, 0);
    std::copy(plain, plain + len, buf.begin() + crypto_box_ZEROBYTES);
    std::vector<u8> out(buf.size(), 0);
    if (crypto_box(out.data(), buf.data(), buf.size(), nonce, peerPublic,
                   ownSecret) != 0) {
        throw std::runtime_error("pepperBoxSeal failed");
    }
    return std::vector<u8>(out.begin() + crypto_box_BOXZEROBYTES, out.end());
}

// Open: nullopt on auth failure (binary: returns true).
inline std::optional<std::vector<u8>> pepperBoxOpen(
    const u8* cipher, std::size_t len, const u8 nonce[kNonceBytes],
    const u8 peerPublic[kKeyBytes], const u8 ownSecret[kKeyBytes]) {
    if (len < kMacBytes) return std::nullopt;
    std::vector<u8> buf(crypto_box_BOXZEROBYTES + len, 0);
    std::copy(cipher, cipher + len, buf.begin() + crypto_box_BOXZEROBYTES);
    std::vector<u8> out(buf.size(), 0);
    if (crypto_box_open(out.data(), buf.data(), buf.size(), nonce, peerPublic,
                        ownSecret) != 0) {
        return std::nullopt;
    }
    return std::vector<u8>(out.begin() + crypto_box_ZEROBYTES, out.end());
}
#else
inline std::vector<u8> pepperBoxSeal(const u8*, std::size_t,
                                     const u8[kNonceBytes],
                                     const u8[kKeyBytes], const u8[kKeyBytes]) {
    throw SodiumMissing();
}
inline std::optional<std::vector<u8>> pepperBoxOpen(
    const u8*, std::size_t, const u8[kNonceBytes], const u8[kKeyBytes],
    const u8[kKeyBytes]) {
    throw SodiumMissing();
}
#endif

} // namespace titan::crypto
