#pragma once

// SkinPurchaseOptionRewardNotification::encode @0x43cbfc, decode @0x461730,
// getNotificationType @0xa525e0 (-> 65).
// Wire: BaseNotification fields, vint @+48, dataref (nullable @+56),
//   vint @+64.

#include "titan/core/DataReference.cpp"
#include "titan/game/notif/BaseNotification.cpp"

#include <optional>

namespace titan {

class SkinPurchaseOptionRewardNotification : public BaseNotification {
public:
    SkinPurchaseOptionRewardNotification() { type_ = 65; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        DataReference::encodeNullable(s, ref56_);
        s.writeVInt(v64_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        ref56_ = DataReference::decodeNullable(s);
        v64_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 65; }

    i32 v48_ = 0;
    std::optional<DataReference> ref56_;
    i32 v64_ = 0;
};

} // namespace titan
