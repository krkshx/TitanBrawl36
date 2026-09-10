#pragma once

// SCIDBindAccountMessage (10636) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SCIDBindAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10636; }
    const char* getMessageTypeName() const override { return "SCIDBindAccountMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCompressedString> f1_, f2_;
};

} // namespace titan

// SCIDBindAccountMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void SCIDBindAccountMessage::encode() {
PiranhaMessage::encode();
    encodeCompressed(stream(), f1_, "SCIDBindAccountMessage needs LogicCompressedString");
    encodeCompressed(stream(), f2_, "SCIDBindAccountMessage needs LogicCompressedString");
}

inline void SCIDBindAccountMessage::decode() {
PiranhaMessage::decode();
    decodeCompressed(stream(), f1_);
    decodeCompressed(stream(), f2_);
}


} // namespace titan
