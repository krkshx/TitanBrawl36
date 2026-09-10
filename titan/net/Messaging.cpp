#pragma once

// Transport framing — reversed from libg_decrypted.so (ARM64).
//
// Every message goes on the wire as:
//   [u16be type][u24be payloadLen][u16be version][payload...]
// That's a 7-byte header (Messaging::writeHeader @0x46664c,
// Messaging::readHeader @0x93da60). payloadLen covers the payload only
// and must be < 0x1000000 (else Debugger::error in the binary).
// The same layout is used by TCP (Messaging::encryptAndWrite @0x93221c)
// and HTTP (HTTPMessaging::writeMessage @0x562efc,
// HTTPMessaging::readMessage @0x5f6480).

#include "titan/core/PiranhaMessage.cpp"
#include "titan/crypto/Encrypter.cpp"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace titan::net {

constexpr int kHeaderSize = 7;
constexpr int kMaxPayload = 0x1000000;

// @0x46664c — Messaging::writeHeader(msg, out7bytes, payloadLen)
inline void writeHeader(const PiranhaMessage& msg, u8 out[kHeaderSize], int payloadLen) {
    if (payloadLen < 0 || payloadLen >= kMaxPayload) {
        throw std::length_error("Messaging::writeHeader: payload too large");
    }
    const auto type = static_cast<u32>(msg.getMessageType());
    const auto ver = static_cast<u32>(msg.getMessageVersion());
    const auto len = static_cast<u32>(payloadLen);
    out[0] = static_cast<u8>((type >> 8) & 0xFF); // BYTE1(v6)
    out[1] = static_cast<u8>(type & 0xFF);
    out[2] = static_cast<u8>((len >> 16) & 0xFF); // BYTE2(a3)
    out[3] = static_cast<u8>((len >> 8) & 0xFF);  // BYTE1(a3)
    out[4] = static_cast<u8>(len & 0xFF);
    const u32 swapped = __builtin_bswap32(ver);   // bswap32 >> 16, LE store
    out[5] = static_cast<u8>((swapped >> 16) & 0xFF); // HI(version)
    out[6] = static_cast<u8>((swapped >> 24) & 0xFF); // LO(version)
}

struct Header {
    int type = 0;
    int length = 0;
    int version = 0;
};

// @0x93da60 — Messaging::readHeader(bytes7, &type, &len, &version)
inline Header readHeader(const u8 in[kHeaderSize]) {
    Header h;
    h.type = (in[0] << 8) | in[1];
    h.length = (in[2] << 16) | (in[3] << 8) | in[4];
    h.version = (in[5] << 8) | in[6];
    return h;
}

// Frame an already-encoded message: header + payload copy.
inline std::vector<u8> encodeFrame(PiranhaMessage& msg) {
    msg.encode();
    const ByteStream& s = msg.getMessageBytes();
    const int len = s.getLength();
    std::vector<u8> out(kHeaderSize + static_cast<std::size_t>(len));
    writeHeader(msg, out.data(), len);
    for (int i = 0; i < len; ++i) out[kHeaderSize + i] = s.data()[i];
    return out;
}

// Send-path frame — Messaging::encryptAndWrite @0x93221c (TCP; the actual
// asio socket write is platform code and lives outside titan_core).
//
// Binary behavior, in order:
//   1. msg.encode(); len = getEncodingLength().
//   2. ClientHello (10100) / Login (10101) go out as a plaintext memcpy;
//      every other type goes through the session Encrypter
//      (slot+24 encrypt, slot+32 overhead added to the length).
//   3. 7-byte header over the FINAL (post-encrypt) length; length
//      >= 0x1000000 trips Debugger::error, hence the throw below.
//
// A null encrypter means "no session yet" and behaves like the binary's
// pre-login path: plaintext body.
inline std::vector<u8> encodeSendFrame(PiranhaMessage& msg, crypto::Encrypter* enc) {
    msg.encode();
    const ByteStream& s = msg.getMessageBytes();
    const int len = s.getLength();
    const int type = msg.getMessageType();

    std::vector<u8> body;
    if (enc != nullptr && type != 10100 && type != 10101) {
        body = enc->encrypt(s.data(), static_cast<std::size_t>(len));
    } else {
        body.assign(s.data(), s.data() + len);
    }
    std::vector<u8> out(kHeaderSize + body.size());
    writeHeader(msg, out.data(), static_cast<int>(body.size()));
    for (std::size_t i = 0; i < body.size(); ++i) out[kHeaderSize + i] = body[i];
    return out;
}

} // namespace titan::net
