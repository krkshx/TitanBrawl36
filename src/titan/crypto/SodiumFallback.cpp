#pragma once
// Замена libsodium, когда TITAN_WITH_SODIUM выключен (на Windows системного
// sodium нет, пакетных менеджеров тоже — тащить его вендором ради четырёх
// функций нет смысла). Покрывает ровно тот API, что зовёт PepperCrypto и
// TweetNaCl19: sodium_init, crypto_scalarmult[_base], crypto_generichash_*.
// X25519 — классический Montgomery-ladder (RFC 7748), BLAKE2b — эталон
// (RFC 7693). Проверены векторами PyNaCl/hashlib (см. стенд cryptotest).
#ifndef TITAN_WITH_SODIUM
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

inline int sodium_init(void) {
    return 0;
}

namespace sodiumfb {

struct Fe {
    // Значение = SUM l[i]*2^(32i) mod (2^255-19), лимбы держим < 2^32 через norm.
    std::uint64_t l[8];
};

static void fe_norm(Fe &f) {
    for (;;) {
        for (int i = 0; i < 7; i++) {
            f.l[i + 1] += f.l[i] >> 32;
            f.l[i] &= 0xFFFFFFFFu;
        }
        std::uint64_t hi = f.l[7] >> 32;
        f.l[7] &= 0xFFFFFFFFu;
        if (hi == 0) {
            break;
        }
        // 2^256 = 2 * 2^255 == 2 * 19 = 38 (mod p), НЕ 19 (это 2^255).
        f.l[0] += hi * 38;
    }
}

static void fe_add(Fe &r, const Fe &a, const Fe &b) {
    for (int i = 0; i < 8; i++) {
        r.l[i] = a.l[i] + b.l[i];
    }
    fe_norm(r);
}

static void fe_sub(Fe &r, const Fe &a, const Fe &b) {
    // p = 2^255-19 лимбами; +4p чтобы не уйти в минус: a-b > -2^256, а 4p > 2^256,
    // итог всегда >= 0 (с +2p при большом b-a уходил в обёртку u64).
    static const std::uint64_t kP[8] = {
        0xFFFFFFEDu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
        0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x7FFFFFFFu
    };
    for (int i = 0; i < 8; i++) {
        r.l[i] = a.l[i] - b.l[i] + 4 * kP[i];
    }
    fe_norm(r);
}

static void fe_mul(Fe &r, const Fe &a, const Fe &b) {
    __uint128_t t[16] = {0};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            t[i + j] += (__uint128_t)a.l[i] * b.l[j];
        }
    }
    // Разносим в 32-битные лимбы; carry за 2^512 сворачиваем через 38^2 = 1444
    // (2^512 = (2^256)^2 == 38^2 mod p).
    std::uint64_t d[16];
    std::uint64_t carry = 0;
    for (int i = 0; i < 16; i++) {
        __uint128_t v = t[i] + carry;
        d[i] = (std::uint64_t)v & 0xFFFFFFFFu;
        carry = (std::uint64_t)(v >> 32);
    }
    // Свёртка: d[i+8] весит 2^256 относительно d[i], а 2^256 == 38 (mod p).
    for (int i = 0; i < 8; i++) {
        r.l[i] = d[i] + (__uint128_t)38 * d[i + 8];
    }
    r.l[0] += (__uint128_t)1444 * carry;
    fe_norm(r);
}

static void fe_sq(Fe &r, const Fe &a) {
    fe_mul(r, a, a);
}

static Fe fe_zero() {
    Fe f = {};
    return f;
}

static Fe fe_one() {
    Fe f = {};
    f.l[0] = 1;
    return f;
}

// Инверсия: x^(2^255-21). Двоичная запись показателя (биты 0..254):
// 2^255-21 = ...11101011b — все единицы, кроме битов 2 и 4.
static void fe_inv(Fe &r, const Fe &x) {
    Fe base = x;
    Fe acc = fe_one();
    for (int i = 254; i >= 0; i--) {
        fe_sq(acc, acc);
        if (i != 2 && i != 4) {
            fe_mul(acc, acc, base);
        }
    }
    r = acc;
}

static void fe_cswap(Fe &a, Fe &b, int bit) {
    std::uint64_t mask = static_cast<std::uint64_t>(-static_cast<std::int64_t>(bit & 1));
    for (int i = 0; i < 8; i++) {
        std::uint64_t d = mask & (a.l[i] ^ b.l[i]);
        a.l[i] ^= d;
        b.l[i] ^= d;
    }
}

// Полная редукция в [0, p): norm оставляет [0, 2^256) — это до ~2p,
// поэтому одного условного вычитания мало (было: p+1 паковалось как есть).
static void fe_reduce(Fe &f) {
    fe_norm(f);
    static const std::uint64_t kP[8] = {
        0xFFFFFFEDu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
        0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x7FFFFFFFu
    };
    for (int k = 0; k < 4; k++) {
        std::uint64_t q[8];
        std::uint64_t borrow = 0;
        for (int i = 0; i < 8; i++) {
            __uint128_t t = (__uint128_t)f.l[i] - kP[i] - borrow;
            q[i] = (std::uint64_t)t & 0xFFFFFFFFu;
            borrow = (t >> 64) & 1u;
        }
        if (borrow != 0) {
            break;
        }
        for (int i = 0; i < 8; i++) {
            f.l[i] = q[i];
        }
    }
}

static void fe_pack(std::uint8_t out[32], const Fe &a) {
    Fe v = a;
    fe_reduce(v);
    for (int i = 0; i < 8; i++) {
        out[4 * i + 0] = static_cast<std::uint8_t>(v.l[i]);
        out[4 * i + 1] = static_cast<std::uint8_t>(v.l[i] >> 8);
        out[4 * i + 2] = static_cast<std::uint8_t>(v.l[i] >> 16);
        out[4 * i + 3] = static_cast<std::uint8_t>(v.l[i] >> 24);
    }
}

static void fe_unpack(Fe &r, const std::uint8_t in[32]) {
    for (int i = 0; i < 8; i++) {
        r.l[i] = static_cast<std::uint64_t>(in[4 * i])
            | (static_cast<std::uint64_t>(in[4 * i + 1]) << 8)
            | (static_cast<std::uint64_t>(in[4 * i + 2]) << 16)
            | (static_cast<std::uint64_t>(in[4 * i + 3]) << 24);
    }
}

// Montgomery-ladder X25519. n — 32 байта LE (без clamp), p — точка.
static void x25519(std::uint8_t out[32], const std::uint8_t n[32], const std::uint8_t p[32]) {
    static const std::uint64_t kA24 = 121665;
    Fe x1, x2 = fe_one(), z2 = fe_zero(), x3, z3 = fe_one();
    fe_unpack(x1, p);
    x3 = x1;
    Fe a24 = fe_zero();
    a24.l[0] = kA24;
    int swap = 0;
    // Все 256 бит, включая старший: для unclamped-скаляров он значим,
    // для clamp-нутых равен 0 (нулевой бит — неподвижная точка лестницы).
    for (int t = 255; t >= 0; t--) {
        int k = (n[t >> 3] >> (t & 7)) & 1;
        swap ^= k;
        fe_cswap(x2, x3, swap);
        fe_cswap(z2, z3, swap);
        swap = k;
        Fe a, aa, b, bb, e, c, d, da, cb;
        fe_add(a, x2, z2);
        fe_sq(aa, a);
        fe_sub(b, x2, z2);
        fe_sq(bb, b);
        fe_sub(e, aa, bb);
        fe_add(c, x3, z3);
        fe_sub(d, x3, z3);
        fe_mul(da, d, a);
        fe_mul(cb, c, b);
        Fe t0, t1;
        fe_add(t0, da, cb);
        fe_sq(x3, t0);
        fe_sub(t1, da, cb);
        fe_sq(t0, t1);
        fe_mul(z3, t0, x1);
        fe_mul(x2, aa, bb);
        Fe t2;
        fe_mul(t2, a24, e);
        fe_add(t1, aa, t2);
        fe_mul(z2, e, t1);
    }
    fe_cswap(x2, x3, swap);
    fe_cswap(z2, z3, swap);
    Fe zi, q;
    fe_inv(zi, z2);
    fe_mul(q, x2, zi);
    fe_pack(out, q);
}

static void x25519_base(std::uint8_t out[32], const std::uint8_t n[32]) {
    std::uint8_t clamped[32];
    std::memcpy(clamped, n, 32);
    clamped[0] &= 248;
    clamped[31] &= 127;
    clamped[31] |= 64;
    static const std::uint8_t kBase[32] = {9};
    x25519(out, clamped, kBase);
}

// --- BLAKE2b (RFC 7693), outlen 1..64 ---

static const std::uint64_t kBlakeIv[8] = {
    0x6a09e667f3bcc908u, 0xbb67ae8584caa73bu,
    0x3c6ef372fe94f82bu, 0xa54ff53a5f1d36f1u,
    0x510e527fade682d1u, 0x9b05688c2b3e6c1fu,
    0x1f83d9abfb41bd6bu, 0x5be0cd19137e2179u
};

static const std::uint8_t kBlakeSigma[10][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
    {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
    {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
    {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
    {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
    {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
    {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
    {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
    {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0}
};

static std::uint64_t rotr64(std::uint64_t x, int c) {
    return (x >> c) | (x << (64 - c));
}

struct Blake2b {
    std::uint64_t h[8];
    std::uint64_t t[2];
    std::uint64_t f[2];
    std::uint8_t buf[128];
    std::size_t buflen = 0;
    std::size_t outlen = 0;
};

static void blake_compress(Blake2b &s, const std::uint8_t block[128]) {
    std::uint64_t m[16];
    for (int i = 0; i < 16; i++) {
        m[i] = static_cast<std::uint64_t>(block[8 * i])
            | (static_cast<std::uint64_t>(block[8 * i + 1]) << 8)
            | (static_cast<std::uint64_t>(block[8 * i + 2]) << 16)
            | (static_cast<std::uint64_t>(block[8 * i + 3]) << 24)
            | (static_cast<std::uint64_t>(block[8 * i + 4]) << 32)
            | (static_cast<std::uint64_t>(block[8 * i + 5]) << 40)
            | (static_cast<std::uint64_t>(block[8 * i + 6]) << 48)
            | (static_cast<std::uint64_t>(block[8 * i + 7]) << 56);
    }
    std::uint64_t v[16];
    for (int i = 0; i < 8; i++) {
        v[i] = s.h[i];
        v[i + 8] = kBlakeIv[i];
    }
    v[12] ^= s.t[0];
    v[13] ^= s.t[1];
    if (s.f[0] != 0) {
        v[14] = ~v[14];
    }
    for (int r = 0; r < 12; r++) {
        const std::uint8_t *sx = kBlakeSigma[r % 10];
#define BLAKE_G(a, b, c, d, x, y) \
    v[a] = v[a] + v[b] + (x); \
    v[d] = rotr64(v[d] ^ v[a], 32); \
    v[c] = v[c] + v[d]; \
    v[b] = rotr64(v[b] ^ v[c], 24); \
    v[a] = v[a] + v[b] + (y); \
    v[d] = rotr64(v[d] ^ v[a], 16); \
    v[c] = v[c] + v[d]; \
    v[b] = rotr64(v[b] ^ v[c], 63);
        BLAKE_G(0, 4, 8, 12, m[sx[0]], m[sx[1]]);
        BLAKE_G(1, 5, 9, 13, m[sx[2]], m[sx[3]]);
        BLAKE_G(2, 6, 10, 14, m[sx[4]], m[sx[5]]);
        BLAKE_G(3, 7, 11, 15, m[sx[6]], m[sx[7]]);
        BLAKE_G(0, 5, 10, 15, m[sx[8]], m[sx[9]]);
        BLAKE_G(1, 6, 11, 12, m[sx[10]], m[sx[11]]);
        BLAKE_G(2, 7, 8, 13, m[sx[12]], m[sx[13]]);
        BLAKE_G(3, 4, 9, 14, m[sx[14]], m[sx[15]]);
#undef BLAKE_G
    }
    for (int i = 0; i < 8; i++) {
        s.h[i] ^= v[i] ^ v[i + 8];
    }
}

static void blake_inc(Blake2b &s, std::size_t n) {
    s.t[0] += static_cast<std::uint64_t>(n);
    if (s.t[0] < static_cast<std::uint64_t>(n)) {
        s.t[1]++;
    }
}

static bool blake_init(Blake2b &s, std::size_t outlen, const std::uint8_t *key, std::size_t keylen) {
    if (outlen < 1 || outlen > 64 || keylen > 64) {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        s.h[i] = kBlakeIv[i];
    }
    s.h[0] ^= 0x01010000u ^ (static_cast<std::uint64_t>(keylen) << 8) ^ static_cast<std::uint64_t>(outlen);
    s.t[0] = 0;
    s.t[1] = 0;
    s.f[0] = 0;
    s.f[1] = 0;
    s.buflen = 0;
    s.outlen = outlen;
    if (keylen > 0) {
        std::uint8_t block[128] = {0};
        std::memcpy(block, key, keylen);
        blake_inc(s, 128);
        blake_compress(s, block);
    }
    return true;
}

static void blake_update(Blake2b &s, const std::uint8_t *in, std::size_t n) {
    while (n > 0) {
        std::size_t room = 128 - s.buflen;
        if (room == 0) {
            blake_inc(s, 128);
            blake_compress(s, s.buf);
            s.buflen = 0;
            room = 128;
        }
        std::size_t take = n < room ? n : room;
        std::memcpy(s.buf + s.buflen, in, take);
        s.buflen += take;
        in += take;
        n -= take;
    }
}

static void blake_final(Blake2b &s, std::uint8_t *out) {
    blake_inc(s, s.buflen);
    s.f[0] = ~std::uint64_t(0);
    std::memset(s.buf + s.buflen, 0, 128 - s.buflen);
    blake_compress(s, s.buf);
    std::uint8_t full[64];
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            full[8 * i + k] = static_cast<std::uint8_t>(s.h[i] >> (8 * k));
        }
    }
    std::memcpy(out, full, s.outlen);
}

} // namespace sodiumfb

struct crypto_generichash_state {
    sodiumfb::Blake2b inner;
};

inline int crypto_generichash_init(crypto_generichash_state *state,
    const unsigned char *key, std::size_t keylen, std::size_t outlen) {
    return sodiumfb::blake_init(state->inner, outlen, key, keylen) ? 0 : -1;
}

inline int crypto_generichash_update(crypto_generichash_state *state,
    const unsigned char *in, unsigned long long inlen) {
    sodiumfb::blake_update(state->inner, in, static_cast<std::size_t>(inlen));
    return 0;
}

inline int crypto_generichash_final(crypto_generichash_state *state,
    unsigned char *out, std::size_t outlen) {
    if (outlen != state->inner.outlen) {
        return -1;
    }
    sodiumfb::blake_final(state->inner, out);
    return 0;
}

inline int crypto_scalarmult(unsigned char *q, const unsigned char *n, const unsigned char *p) {
    // libsodium клампит скаляр и здесь (проверено: n=1,2,3 дают один результат).
    std::uint8_t clamped[32];
    std::memcpy(clamped, n, 32);
    clamped[0] &= 248;
    clamped[31] &= 127;
    clamped[31] |= 64;
    sodiumfb::x25519(q, clamped, p);
    return 0;
}

inline int crypto_scalarmult_base(unsigned char *q, const unsigned char *n) {
    sodiumfb::x25519_base(q, n);
    return 0;
}

#endif // TITAN_WITH_SODIUM
