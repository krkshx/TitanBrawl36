// BillingCancelledByClientMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.

#include "titan/messages/BillingCancelledByClientMessage.hpp"

namespace titan {

void BillingCancelledByClientMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(productIds_.size()));
    for (const auto& p : productIds_) stream().writeString(&p);
}

void BillingCancelledByClientMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    productIds_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) productIds_.push_back(stream().readString().value_or(""));
}


} // namespace titan
