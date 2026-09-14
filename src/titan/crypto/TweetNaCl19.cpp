#pragma once
// TweetNaCl-порт pepper-крипты, снятый с либы.
// Ключевое отличие от стандарта: Core крутит 19 итераций вместо 20
// (в либе нет строки "expand 32-byte k" среди стандартных констант,
// стоковый клиент сходится только с 19-раундовым ядром).
// Остальное (раскладка HSalsa/Salsa, Poly1305, скалярка) — как в оригинале.
#include <cstdint>
#include <cstring>
#include <vector>
#ifdef TITAN_WITH_SODIUM
#include <sodium.h>
#else
#include "SodiumFallback.cpp"
#endif

namespace tweet19 {

inline std::uint32_t L32(std::uint32_t x, int c) {
    return (x << c) | (x >> (32 - c));
}

inline std::uint32_t Ld32(const std::uint8_t *x, int off) {
    return static_cast<std::uint32_t>(x[off])
        | (static_cast<std::uint32_t>(x[off + 1]) << 8)
        | (static_cast<std::uint32_t>(x[off + 2]) << 16)
        | (static_cast<std::uint32_t>(x[off + 3]) << 24);
}

inline void St32(std::uint8_t *x, std::uint32_t u, int off) {
    for (int i = 0; i < 4; i++) {
        x[i + off] = static_cast<std::uint8_t>(u);
        u >>= 8;
    }
}

inline void Core(std::uint8_t *pout, const std::uint8_t *pin, const std::uint8_t *k,
                 const std::uint8_t *c, bool hsalsa) {
    std::uint32_t w[16], x[16], y[16], t[4];
    for (int i = 0; i < 4; i++) {
        x[5 * i] = Ld32(c, 4 * i);
        x[1 + i] = Ld32(k, 4 * i);
        x[6 + i] = Ld32(pin, 4 * i);
        x[11 + i] = Ld32(k, 16 + 4 * i);
    }
    for (int i = 0; i < 16; ++i) {
        y[i] = x[i];
    }
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int m = 0; m < 4; ++m) {
                t[m] = x[(5 * j + 4 * m) % 16];
            }
            t[1] ^= L32(t[0] + t[3], 7);
            t[2] ^= L32(t[1] + t[0], 9);
            t[3] ^= L32(t[2] + t[1], 13);
            t[0] ^= L32(t[3] + t[2], 18);
            for (int m = 0; m < 4; ++m) {
                w[4 * j + (j + m) % 4] = t[m];
            }
        }
        for (int m = 0; m < 16; ++m) {
            x[m] = w[m];
        }
    }
    if (hsalsa) {
        for (int i = 0; i < 16; ++i) {
            x[i] += y[i];
        }
        for (int i = 0; i < 4; ++i) {
            x[5 * i] -= Ld32(c, 4 * i);
            x[6 + i] -= Ld32(pin, 4 * i);
        }
        for (int i = 0; i < 4; ++i) {
            St32(pout, x[5 * i], 4 * i);
            St32(pout, x[6 + i], 16 + 4 * i);
        }
    } else {
        for (int i = 0; i < 16; ++i) {
            St32(pout, x[i] + y[i], 4 * i);
        }
    }
}

static const std::uint8_t SIGMA[16] = {
    'e', 'x', 'p', 'a', 'n', 'd', ' ', '3',
    '2', '-', 'b', 'y', 't', 'e', ' ', 'k'
};

inline void HSalsa20(std::uint8_t out[32], const std::uint8_t in16[16],
                     const std::uint8_t key[32]) {
    Core(out, in16, key, SIGMA, true);
}

// XSalsa20-поток: sub = HSalsa20(nonce[0:16], key),
// затем Salsa20 c nonce[16:24] и счётчиком (как CryptoStreamXor в оригинале).
inline void SalsaXor(const std::uint8_t *msg, std::uint8_t *out, std::int64_t len,
                     const std::uint8_t nonce[24], const std::uint8_t key[32]) {
    std::uint8_t sub[32];
    HSalsa20(sub, nonce, key);
    std::uint8_t z[16] = {0};
    std::uint8_t x[64];
    for (int i = 0; i < 8; ++i) {
        z[i] = nonce[16 + i];
    }
    std::int64_t mOff = 0;
    std::uint32_t u = 0;
    while (len >= 64) {
        Core(x, z, sub, SIGMA, false);
        for (int i = 0; i < 64; ++i) {
            out[mOff + i] = static_cast<std::uint8_t>(msg[mOff + i] ^ x[i]);
        }
        u = 1;
        for (int i = 8; i < 16; ++i) {
            u += z[i];
            z[i] = static_cast<std::uint8_t>(u);
            u >>= 8;
        }
        len -= 64;
        mOff += 64;
    }
    if (len != 0) {
        Core(x, z, sub, SIGMA, false);
        for (std::int64_t i = 0; i < len; i++) {
            out[mOff + i] = static_cast<std::uint8_t>(msg[mOff + i] ^ x[i]);
        }
    }
}

// --- Poly1305 (donna, как в оригинале: 320-фактор, Minusp) ---
namespace poly {
inline void Add1305(std::uint32_t h[17], const std::uint32_t c[17]) {
    std::uint32_t u = 0;
    for (int j = 0; j < 17; j++) {
        u += h[j] + c[j];
        h[j] = u & 255;
        u >>= 8;
    }
}
static const std::uint32_t MINUSP[17] = {
    5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252
};
inline void OnetimeAuth(std::uint8_t *out, int outOff, const std::uint8_t *m, int mOff,
                        std::int64_t n, const std::uint8_t *k) {
    std::uint32_t x[17], h[17] = {0}, r[17] = {0}, g[17], c[17];
    for (int j = 0; j < 16; j++) {
        r[j] = k[j];
    }
    r[3] &= 15; r[4] &= 252; r[7] &= 15; r[8] &= 252;
    r[11] &= 15; r[12] &= 252; r[15] &= 15;
    while (n > 0) {
        for (int j = 0; j < 17; j++) {
            c[j] = 0;
        }
        std::int64_t j = 0;
        for (; j < 16 && j < n; ++j) {
            c[j] = m[mOff + j];
        }
        c[j] = 1;
        mOff += j;
        n -= j;
        Add1305(h, c);
        for (int i = 0; i < 17; ++i) {
            x[i] = 0;
            for (int jj = 0; jj < 17; ++jj) {
                x[i] += h[jj] * ((jj <= i) ? r[i - jj] : 320u * r[i + 17 - jj]);
            }
        }
        for (int i = 0; i < 17; ++i) {
            h[i] = x[i];
        }
        std::uint32_t u = 0;
        for (int j = 0; j < 16; ++j) {
            u += h[j];
            h[j] = u & 255;
            u >>= 8;
        }
        u += h[16];
        h[16] = u & 3;
        u = 5 * (u >> 2);
        for (int j = 0; j < 16; ++j) {
            u += h[j];
            h[j] = u & 255;
            u >>= 8;
        }
        u += h[16];
        h[16] = u;
    }
    for (int j = 0; j < 17; j++) {
        g[j] = h[j];
    }
    Add1305(h, MINUSP);
    std::uint32_t s = -(h[16] >> 7);
    for (int j = 0; j < 17; j++) {
        h[j] ^= s & (g[j] ^ h[j]);
    }
    for (int j = 0; j < 16; j++) {
        c[j] = k[j + 16];
    }
    c[16] = 0;
    Add1305(h, c);
    for (int j = 0; j < 16; j++) {
        out[outOff + j] = static_cast<std::uint8_t>(h[j]);
    }
}
inline bool OnetimeVerify(const std::uint8_t *h, int hOff, const std::uint8_t *m, int mOff,
                          std::int64_t n, const std::uint8_t *k) {
    std::uint8_t x[16];
    std::vector<std::uint8_t> tmp(16);
    OnetimeAuth(tmp.data(), 0, m, mOff, n, k);
    std::uint32_t d = 0;
    for (int j = 0; j < 16; j++) {
        d |= tmp[j] ^ h[hOff + j];
    }
    (void)x;
    return d == 0;
}
} // namespace poly

// --- secretbox/box поверх 19-раундового ядра (раскладка паддингов — как в либе:
// seal: 32 нуля + msg -> стрим, тэг в [16:32], наружу с 16; open зеркально) ---

inline void SecretboxSeal(const std::uint8_t *msg, std::uint8_t *out, std::int64_t len,
                          const std::uint8_t nonce[24], const std::uint8_t key[32]) {
    std::vector<std::uint8_t> padded(static_cast<std::size_t>(len + 32), 0);
    std::memcpy(padded.data() + 32, msg, static_cast<std::size_t>(len));
    std::vector<std::uint8_t> ciphered(static_cast<std::size_t>(len + 32));
    SalsaXor(padded.data(), ciphered.data(), len + 32, nonce, key);
    poly::OnetimeAuth(ciphered.data(), 16, ciphered.data(), 32, len, ciphered.data());
    std::memset(ciphered.data(), 0, 16);
    std::memcpy(out, ciphered.data() + 16, static_cast<std::size_t>(len + 16));
}

inline bool SecretboxOpen(const std::uint8_t *box, std::uint8_t *out, std::int64_t len,
                          const std::uint8_t nonce[24], const std::uint8_t key[32]) {
    if (len < 16) {
        return false;
    }
    std::vector<std::uint8_t> padded(static_cast<std::size_t>(len + 16), 0);
    std::memcpy(padded.data() + 16, box, static_cast<std::size_t>(len));
    std::uint8_t nonceKey[32];
    {
        std::uint8_t zeros[32] = {0};
        SalsaXor(zeros, nonceKey, 32, nonce, key);
    }
    if (!poly::OnetimeVerify(padded.data() + 16, 0, padded.data() + 32,
                             0, len + 16 - 32, nonceKey)) {
        return false;
    }
    std::vector<std::uint8_t> dec(static_cast<std::size_t>(len + 16));
    SalsaXor(padded.data(), dec.data(), len + 16, nonce, key);
    std::memcpy(out, dec.data() + 32, static_cast<std::size_t>(len - 16));
    return true;
}

// Shared = HSalsa19(scalarmult(secret, public)). Скалярка — sodium (побайтово
// равна либе: base и mult сверены).
inline void BoxBeforenm(const std::uint8_t pub[32], const std::uint8_t sec[32],
                        std::uint8_t shared[32]) {
    std::uint8_t s[32];
    if (crypto_scalarmult(s, sec, pub) != 0) {
        std::memset(shared, 0, 32);
        return;
    }
    static const std::uint8_t zero16[16] = {0};
    HSalsa20(shared, zero16, s);
}

inline void BoxSeal(const std::uint8_t *msg, std::uint8_t *out, std::int64_t len,
                    const std::uint8_t nonce[24], const std::uint8_t pub[32],
                    const std::uint8_t sec[32]) {
    std::uint8_t k[32];
    BoxBeforenm(pub, sec, k);
    SecretboxSeal(msg, out, len, nonce, k);
}

inline bool BoxOpen(const std::uint8_t *box, std::uint8_t *out, std::int64_t len,
                    const std::uint8_t nonce[24], const std::uint8_t pub[32],
                    const std::uint8_t sec[32]) {
    std::uint8_t k[32];
    BoxBeforenm(pub, sec, k);
    return SecretboxOpen(box, out, len, nonce, k);
}

} // namespace tweet19
