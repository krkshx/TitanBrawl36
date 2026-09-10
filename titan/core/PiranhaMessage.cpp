#pragma once

#include "titan/core/ByteStream.cpp"

// PiranhaMessage — base of every network message in libg.so.
//
// vtable layout (ARM64, 8-byte slots, verified in docs/IDA_BASELINE.md):
//   0:D2 1:D0 2:encode 3:decode 4:getServiceNodeType
//   5:getMessageType 6:getMessageTypeName 7:destruct
// The embedded ByteStream sits at this+16 on ARM64; the message version
// lives at this+8 (u16, read by getMessageVersion @0x571a94).
//
// PiranhaMessage::encode @0x8f2e70 is an anti-tamper return-address guard
// (AntiCheat::guard_callback(0x1F) on violation) and otherwise a no-op —
// every derived encode() calls it first, then writes its fields.

namespace titan {

class PiranhaMessage {
public:
    PiranhaMessage();
    virtual ~PiranhaMessage();

    PiranhaMessage(const PiranhaMessage&) = delete;
    PiranhaMessage& operator=(const PiranhaMessage&) = delete;

    virtual void encode(); // @0x8f2e70 (guard + no-op)
    virtual void decode();
    virtual int getServiceNodeType() const { return 1; }
    virtual int getMessageType() const = 0;
    virtual const char* getMessageTypeName() const { return "PiranhaMessage"; }
    virtual void destruct() {}

    // @0x610870 — returns the embedded stream (this+16).
    ByteStream& getMessageBytes() { return stream_; }
    const ByteStream& getMessageBytes() const { return stream_; }
    // @0x571a94 — reads the u16 version at this+8.
    [[nodiscard]] int getMessageVersion() const { return version_; }
    void setMessageVersion(int v) { version_ = v & 0xFFFF; }
    // @0x247de0 — payload length after encode().
    [[nodiscard]] int getEncodingLength() const { return stream_.getLength(); }

    ByteStream& stream() { return stream_; }
    const ByteStream& stream() const { return stream_; }

protected:
    int version_ = 0;   // == *(u16*)(this+8) in the binary
    ByteStream stream_; // == this+16 in the binary
};

} // namespace titan

// PiranhaMessage — reversed from libg_decrypted.so (ARM64).


namespace titan {

inline PiranhaMessage::PiranhaMessage() = default;
inline PiranhaMessage::~PiranhaMessage() = default;

// @0x8f2e70 — PiranhaMessage::encode
// The binary verifies the return address lies inside libg's address range
// (_libg_begin_ptr.._libg_end_ptr) and calls AntiCheat::guard_callback(0x1F)
// otherwise. Semantically a no-op: no bytes are written here.
// We keep the hook point (TITAN_ANTITAMPER_HOOK) for ports that re-add it.
inline void PiranhaMessage::encode() {
#ifdef TITAN_ANTITAMPER_HOOK
    TITAN_ANTITAMPER_HOOK(0x1F);
#else
    (void)0;
#endif
}

inline void PiranhaMessage::decode() {
    // Base decode is empty in the binary (ResetAccountMessage::decode @0x93d3c8
    // calls PiranhaMessage::decode() then reads its own field).
}

} // namespace titan

