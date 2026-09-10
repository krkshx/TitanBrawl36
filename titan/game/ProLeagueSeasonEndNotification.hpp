#pragma once

// ProLeagueSeasonEndNotification::encode @0x219370, decode @0x366de4,
// getNotificationType @0xa70894 (-> 77).
// Wire: BaseNotification fields, then vint @+48..+64.
// Decode mirrors the same order.

#include "titan/game/BaseNotification.hpp"

namespace titan {

class ProLeagueSeasonEndNotification : public BaseNotification {
public:
    ProLeagueSeasonEndNotification() { type_ = 77; }

    void encode(ByteStream& s) const override {
        BaseNotification::encode(s);
        s.writeVInt(v48_);
        s.writeVInt(v52_);
        s.writeVInt(v56_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
    }
    void decode(ByteStream& s) override {
        BaseNotification::decode(s);
        v48_ = s.readVInt();
        v52_ = s.readVInt();
        v56_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
    }
    [[nodiscard]] int notificationType() const override { return 77; }

    i32 v48_ = 0;
    i32 v52_ = 0;
    i32 v56_ = 0;
    i32 v60_ = 0;
    i32 v64_ = 0;
};

} // namespace titan
