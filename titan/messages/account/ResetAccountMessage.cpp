#pragma once

#include "titan/core/PiranhaMessage.cpp"

// ResetAccountMessage (10116) — reversed from libg_decrypted.so.
//   getMessageType @0x6cdf20 -> 10116 (0x2784)
//   encode         @0x4465cc -> PiranhaMessage::encode + writeInt(field+132)
//   decode         @0x93d3c8 -> PiranhaMessage::decode + field = readInt()

namespace titan {

class ResetAccountMessage : public PiranhaMessage {
public:
    ResetAccountMessage();
    ~ResetAccountMessage() override;

    void encode() override;
    void decode() override;
    int getMessageType() const override { return 10116; }
    const char* getMessageTypeName() const override { return "ResetAccountMessage"; }

    int accountPreset() const { return accountPreset_; }
    void setAccountPreset(int v) { accountPreset_ = v; }

private:
    int accountPreset_ = 0; // == *(this+132) in the binary
};

} // namespace titan

// ResetAccountMessage — reversed from libg_decrypted.so (ARM64).


namespace titan {

inline ResetAccountMessage::ResetAccountMessage() = default;
inline ResetAccountMessage::~ResetAccountMessage() = default;

// @0x4465cc — ResetAccountMessage::encode
inline void ResetAccountMessage::encode() {
PiranhaMessage::encode();
    stream().writeInt(accountPreset_);
}

// @0x93d3c8 — ResetAccountMessage::decode
inline void ResetAccountMessage::decode() {
PiranhaMessage::decode();
    accountPreset_ = stream().readInt();
}

} // namespace titan
