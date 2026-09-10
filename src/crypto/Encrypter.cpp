// PepperEncrypter backends.

#include "titan/crypto/Encrypter.hpp"

#ifdef TITAN_WITH_SODIUM
#include <sodium.h>
#endif

#include <cstring>

namespace titan::crypto {

namespace {

#ifdef TITAN_WITH_SODIUM
class SodiumPepper : public Encrypter {
public:
    SodiumPepper(const u8 key[kKeyBytes], const u8 nonce[kNonceBytes]) {
        std::memcpy(key_, key, kKeyBytes);
        std::memcpy(nonce_, nonce, kNonceBytes);
    }
    std::vector<u8> encrypt(const u8* plain, std::size_t len) override {
        // NB: the binary advances the nonce BEFORE use (encrypt @0x4491dc,
        // decrypt @0x39f6b4 call nextNonce first), so the initial nonce
        // value itself never goes on the wire. Mirror that exactly.
        nextNonce(nonce_);
        std::vector<u8> out(len + kMacBytes);
        if (crypto_secretbox_easy(out.data(), plain,
                                  static_cast<unsigned long long>(len),
                                  nonce_, key_) != 0) {
            throw std::runtime_error("secretbox encrypt failed");
        }
        return out;
    }
    std::vector<u8> decrypt(const u8* cipher, std::size_t len) override {
        if (len < kMacBytes) throw std::runtime_error("ciphertext too short");
        nextNonce(nonce_);
        std::vector<u8> out(len - kMacBytes);
        if (crypto_secretbox_open_easy(out.data(), cipher,
                                       static_cast<unsigned long long>(len),
                                       nonce_, key_) != 0) {
            throw std::runtime_error("secretbox MAC failure (send CryptoError)");
        }
        return out;
    }

private:
    u8 key_[kKeyBytes];
    u8 nonce_[kNonceBytes];
};
#endif

} // namespace

std::unique_ptr<Encrypter> makePepperEncrypter(const u8 key[kKeyBytes],
                                              const u8 nonce[kNonceBytes]) {
#ifdef TITAN_WITH_SODIUM
    return std::make_unique<SodiumPepper>(key, nonce);
#else
    (void)key;
    (void)nonce;
    throw SodiumMissing();
#endif
}

} // namespace titan::crypto
