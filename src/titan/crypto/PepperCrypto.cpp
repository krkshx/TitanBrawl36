#pragma once
#include "Random.cpp"
#include <sodium.h>
#include <cstdint>
#include <cstring>
#include <vector>

static const std::uint8_t PEPPER_SERVER_KEY[32] = {
    0xb0, 0x68, 0x5f, 0xe6, 0xb7, 0xca, 0x11, 0x9c,
    0x3a, 0x47, 0x2a, 0x87, 0x86, 0x26, 0x19, 0xa7,
    0x22, 0x60, 0xd1, 0xbb, 0xfb, 0xc4, 0x13, 0x59,
    0xca, 0x61, 0x90, 0x0b, 0xe2, 0xbe, 0x2b, 0x71
};

class PepperCrypto {
public:
    static bool init() {
        static bool ready = false;
        if (!ready) {
            ready = sodium_init() >= 0;
        }
        return ready;
    }

    static void clientSecret(std::uint8_t out[32]) {
        randombytes(out, 32);
    }

    static bool clientPublic(std::uint8_t out[32]) {
        std::uint8_t sk[32];
        clientSecret(sk);
        return crypto_scalarmult_base(out, sk) == 0;
    }

    static bool blake2b24(std::uint8_t out[24], const std::uint8_t *a, int aLen, const std::uint8_t *b, int bLen, const std::uint8_t *c = nullptr, int cLen = 0) {
        crypto_generichash_state st;
        if (crypto_generichash_init(&st, nullptr, 0, 24) != 0) {
            return false;
        }
        crypto_generichash_update(&st, a, static_cast<unsigned long long>(aLen));
        crypto_generichash_update(&st, b, static_cast<unsigned long long>(bLen));
        if (c != nullptr && cLen > 0) {
            crypto_generichash_update(&st, c, static_cast<unsigned long long>(cLen));
        }
        return crypto_generichash_final(&st, out, 24) == 0;
    }

    static bool boxSeal(const std::uint8_t *msg, int msgLen, std::uint8_t *out, const std::uint8_t nonce[24], const std::uint8_t pk[32], const std::uint8_t sk[32]) {
        return crypto_box_easy(out, msg, static_cast<unsigned long long>(msgLen), nonce, pk, sk) == 0;
    }

    static bool boxOpen(const std::uint8_t *box, int boxLen, std::uint8_t *out, const std::uint8_t nonce[24], const std::uint8_t pk[32], const std::uint8_t sk[32]) {
        return crypto_box_open_easy(out, box, static_cast<unsigned long long>(boxLen), nonce, pk, sk) == 0;
    }

    static bool secretSeal(const std::uint8_t *msg, int msgLen, std::uint8_t *out, const std::uint8_t nonce[24], const std::uint8_t key[32]) {
        return crypto_secretbox_easy(out, msg, static_cast<unsigned long long>(msgLen), nonce, key) == 0;
    }

    static bool secretOpen(const std::uint8_t *box, int boxLen, std::uint8_t *out, const std::uint8_t nonce[24], const std::uint8_t key[32]) {
        return crypto_secretbox_open_easy(out, box, static_cast<unsigned long long>(boxLen), nonce, key) == 0;
    }
};

class PepperStream {
public:
    PepperStream() = default;

    void setup(const std::uint8_t key[32], const std::uint8_t nonceBase[24]) {
        std::memcpy(key_, key, 32);
        std::memcpy(nonce_, nonceBase, 24);
    }

    static void nextNonce(std::uint8_t nonce[24]) {
        for (int j = 0; j < 2; j++) {
            unsigned int c = 1;
            for (int i = 0; i < 24; i++) {
                c += nonce[i];
                nonce[i] = static_cast<std::uint8_t>(c & 0xFFu);
                c >>= 8;
            }
        }
    }

    bool encrypt(const std::vector<std::uint8_t> &plain, std::vector<std::uint8_t> &out) {
        nextNonce(nonce_);
        out.resize(plain.size() + 16);
        return PepperCrypto::secretSeal(plain.data(), static_cast<int>(plain.size()), out.data(), nonce_, key_);
    }

    bool decrypt(const std::uint8_t *box, int boxLen, std::vector<std::uint8_t> &out) {
        nextNonce(nonce_);
        if (boxLen < 16) {
            return false;
        }
        out.resize(static_cast<std::size_t>(boxLen - 16));
        return PepperCrypto::secretOpen(box, boxLen, out.data(), nonce_, key_);
    }

private:
    std::uint8_t key_[32] = {0};
    std::uint8_t nonce_[24] = {0};
};
