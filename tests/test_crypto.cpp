// Self-check for session crypto scaffolding.

#include "titan/crypto/Encrypter.hpp"
#include "titan/crypto/PepperBox.hpp"

#include <cstdio>
#include <cstring>

using namespace titan;
using namespace titan::crypto;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    // nextNonce: LE24 += 2.
    {
        u8 n[kNonceBytes] = {};
        nextNonce(n);
        CHECK(n[0] == 2 && n[1] == 0);
        n[0] = 0xFF;
        nextNonce(n); // 0xFF + 2 = 0x101 -> carry
        CHECK(n[0] == 0x01 && n[1] == 0x01);
    }
    CHECK(kMacBytes == 16 && kKeyBytes == 32 && kNonceBytes == 24);
#ifndef TITAN_WITH_SODIUM
    // Without the backend the factory fails loudly, not silently.
    {
        u8 k[kKeyBytes] = {};
        u8 n[kNonceBytes] = {};
        bool threw = false;
        try {
            (void)makePepperEncrypter(k, n);
        } catch (const SodiumMissing&) {
            threw = true;
        }
        CHECK(threw);
    }
    // Without the backend the box calls fail loudly, not silently.
    {
        u8 k[kKeyBytes] = {};
        u8 n[kNonceBytes] = {};
        bool threw = false;
        try {
            (void)pepperBoxSeal(k, 0, n, k, k);
        } catch (const SodiumMissing&) {
            threw = true;
        }
        CHECK(threw);
        threw = false;
        try {
            (void)pepperBoxOpen(k, 0, n, k, k);
        } catch (const SodiumMissing&) {
            threw = true;
        }
        CHECK(threw);
    }
#else
    // With libsodium: secretbox round-trip incl. nonce stepping.
    {
        u8 k[kKeyBytes] = {};
        u8 n[kNonceBytes] = {};
        auto enc = makePepperEncrypter(k, n);
        auto dec = makePepperEncrypter(k, n);
        const u8 plain[] = {1, 2, 3, 4, 5};
        auto cipher = enc->encrypt(plain, sizeof(plain));
        CHECK(cipher.size() == sizeof(plain) + kMacBytes);
        auto back = dec->decrypt(cipher.data(), cipher.size());
        CHECK(back.size() == sizeof(plain) && std::memcmp(back.data(), plain, sizeof(plain)) == 0);
        // MAC failure throws.
        cipher[0] ^= 0xFF;
        bool threw = false;
        try {
            (void)dec->decrypt(cipher.data(), cipher.size());
        } catch (const std::runtime_error&) {
            threw = true;
        }
        CHECK(threw);
    }
    // box_open @0x5863c0 / box @0x3677b0: padded NaCl box round-trip.
    {
        u8 pkA[kKeyBytes], skA[kKeyBytes], pkB[kKeyBytes], skB[kKeyBytes];
        CHECK(crypto_box_keypair(pkA, skA) == 0);
        CHECK(crypto_box_keypair(pkB, skB) == 0);
        u8 nonce[kNonceBytes] = {1};
        const u8 plain[] = {9, 8, 7, 6};
        auto cipher = pepperBoxSeal(plain, sizeof(plain), nonce, pkB, skA);
        CHECK(cipher.size() == sizeof(plain) + kMacBytes);
        auto back = pepperBoxOpen(cipher.data(), cipher.size(), nonce, pkA, skB);
        CHECK(back && back->size() == sizeof(plain)
              && std::memcmp(back->data(), plain, sizeof(plain)) == 0);
        // Tampered cipher -> nullopt (binary: returns true).
        cipher[4] ^= 0xFF;
        CHECK(!pepperBoxOpen(cipher.data(), cipher.size(), nonce, pkA, skB));
        // Wrong peer key -> nullopt.
        u8 pkC[kKeyBytes], skC[kKeyBytes];
        CHECK(crypto_box_keypair(pkC, skC) == 0);
        CHECK(!pepperBoxOpen(cipher.data(), cipher.size(), nonce, pkC, skB));
    }
#endif
    if (failures == 0) std::puts("crypto: all ok");
    return failures == 0 ? 0 : 1;
}
