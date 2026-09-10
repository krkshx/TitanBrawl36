#pragma once

// BandNotification::encode @0x8bb3d0, decode @0x43e904,
// getNotificationType @0x9aca40 (-> 82).
// Wire: BaseNotification fields, then PlayerDisplayData @+48 (required).

#include "titan/game/BaseNotification.hpp"
#include "titan/game/PlayerDisplayData.hpp"

#include <memory>

namespace titan {

class BandNotification : public BaseNotification {
public:
    BandNotification() { type_ = 82; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        if (!display_) throw pending_reverse("BandNotification needs PlayerDisplayData");
        display_->encode(s);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        display_ = std::make_unique<PlayerDisplayData>();
        display_->decode(s);
    }
    [[nodiscard]] int notificationType() const override { return 82; }

    std::unique_ptr<PlayerDisplayData> display_; // +48 (required)
};

} // namespace titan
