#pragma once

// StarPointsNotification::encode @0x542334, decode @0x2e4ab4,
// getNotificationType @0xa54fbc (-> 79).
// Wire: BaseNotification fields, vint count + ScoreEntry[] (@+48, count +60).

#include "titan/game/BaseNotification.hpp"
#include "titan/game/ScoreEntry.hpp"

#include <memory>
#include <vector>

namespace titan {

class StarPointsNotification : public BaseNotification {
public:
    StarPointsNotification() { type_ = 79; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(static_cast<i32>(scores_.size()));
        for (const auto& e : scores_) e->encode(s);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        const i32 n = s.readVInt();
        scores_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto e = std::make_unique<ScoreEntry>();
            e->decode(s);
            scores_.push_back(std::move(e));
        }
    }
    [[nodiscard]] int notificationType() const override { return 79; }

    std::vector<std::unique_ptr<ScoreEntry>> scores_; // +48 (+60)
};

} // namespace titan
