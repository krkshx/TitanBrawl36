#pragma once

// ClientInputMessage (10555) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ClientInputMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10555; }
    const char* getMessageTypeName() const override { return "ClientInputMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0, f136_ = 0, f140_ = 0, f144_ = 0, f148_ = 0;
    std::vector<ClientInput> inputs_;
};

} // namespace titan

// ClientInputMessage bodies — split out of MsgBatch03.cpp; wire format unchanged.


namespace titan {

inline void ClientInputMessage::encode() {
PiranhaMessage::encode();
    BitStream bits(58);
    bits.writePositiveInt(f132_, 14); // Max16383
    bits.writePositiveInt(f136_, 10); // Max1023
    bits.writePositiveInt(f140_, 13); // Max8191 (min in binary; same clamp)
    bits.writePositiveInt(f144_, 10);
    bits.writePositiveInt(f148_, 10);
    bits.writePositiveInt(static_cast<i32>(inputs_.size()), 5); // Max31
    for (const auto& in : inputs_) in.encode(bits);
    const i32 n = bits.getLength();
    stream().writeBytesWithoutLength(bits.getByteArray(), n);
}

inline void ClientInputMessage::decode() {
PiranhaMessage::decode();
    // NOTE: decode reads the trailing BitStream blob; exact binary decode
    // shape pending capture — mirror of encode:
    const i32 n = stream().remaining();
    std::vector<u8> blob = stream().readRawBytes(n);
    BitStream bits;
    bits.setBuffer(blob.data(), static_cast<i32>(blob.size()));
    f132_ = bits.readPositiveInt(14);
    f136_ = bits.readPositiveInt(10);
    f140_ = bits.readPositiveInt(13);
    f144_ = bits.readPositiveInt(10);
    f148_ = bits.readPositiveInt(10);
    const i32 count = bits.readPositiveInt(5);
    inputs_.clear();
    for (i32 i = 0; i < count; ++i) {
        ClientInput in;
        in.decode(bits);
        inputs_.push_back(in);
    }
}


// Wire: string name, string description, dataref badge, dataref region,
// vint allianceType, vint requiredTrophies, bool familyFriendly.

} // namespace titan
