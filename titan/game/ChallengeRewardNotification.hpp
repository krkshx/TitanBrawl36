#pragma once

// ChallengeRewardNotification::encode @0x43b59c, decode @0x478ff0,
// getNotificationType @0x993cf8 (-> 63 if v64_ != 0 else 70; factory cases
// '?' and 'F' both build this class).
// Wire: BaseNotification fields, LogicRewards @+56 (required),
//   vint @+48/+64, string (nullable String* @+72).

#include "titan/game/BaseNotification.hpp"
#include "titan/game/LogicRewards.hpp"

#include <memory>
#include <optional>
#include <string>

namespace titan {

class ChallengeRewardNotification : public BaseNotification {
public:
    ChallengeRewardNotification() { type_ = 63; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        if (!rewards_) throw pending_reverse("ChallengeRewardNotification needs LogicRewards");
        rewards_->encode(s);
        s.writeVInt(v48_);
        s.writeVInt(v64_);
        s.writeString(s72_.has_value() ? &s72_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        rewards_ = std::make_unique<LogicRewards>();
        rewards_->decode(s);
        v48_ = s.readVInt();
        v64_ = s.readVInt();
        s72_ = s.readString();
    }
    [[nodiscard]] int notificationType() const override {
        return v64_ != 0 ? 63 : 70;
    }

    std::unique_ptr<LogicRewards> rewards_; // +56 (required)
    i32 v48_ = 0, v64_ = 0;
    std::optional<std::string> s72_;
};

} // namespace titan
