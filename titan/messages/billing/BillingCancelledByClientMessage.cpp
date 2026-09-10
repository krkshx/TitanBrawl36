#pragma once

// BillingCancelledByClientMessage (10160) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class BillingCancelledByClientMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10160; }
    const char* getMessageTypeName() const override { return "BillingCancelledByClientMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::string> productIds_;
    bool isNull_ = false;
};

} // namespace titan

// BillingCancelledByClientMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.


namespace titan {

inline void BillingCancelledByClientMessage::encode() {
PiranhaMessage::encode();
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(productIds_.size()));
    for (const auto& p : productIds_) stream().writeString(&p);
}

inline void BillingCancelledByClientMessage::decode() {
PiranhaMessage::decode();
    const i32 n = stream().readInt();
    productIds_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) productIds_.push_back(stream().readString().value_or(""));
}


} // namespace titan
