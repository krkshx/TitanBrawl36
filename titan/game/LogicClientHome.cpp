// LogicClientHome bodies (see LogicClientHome.hpp).

#include "titan/game/LogicClientHome.hpp"
#include "titan/game/NotificationFactory.hpp"

#include <string>

namespace titan {

void LogicClientHome::encode(ByteStream& s) const {
    if (!daily_) throw pending_reverse("LogicClientHome needs LogicDailyData");
    daily_->encode(s);
    if (!conf_) throw pending_reverse("LogicClientHome needs LogicConfData");
    conf_->encode(s);
    id_.encode(s);
    s.writeVInt(static_cast<i32>(notifications_.size()));
    for (const auto& [type, n] : notifications_) {
        s.writeVInt(type);
        if (!n) throw pending_reverse("LogicClientHome needs Notification");
        n->encode(s);
    }
    s.writeVInt(v76_);
    s.writeBoolean(b72_);
    if (dropsNull_) {
        s.writeVInt(0);
    } else {
        s.writeVInt(static_cast<i32>(drops_.size()));
        for (const auto& d : drops_) d->encode(s);
    }
    s.writeVInt(static_cast<i32>(refs_.size()));
    for (const auto& r : refs_) r.encode(s);
}

void LogicClientHome::decode(ByteStream& s) {
    daily_ = std::make_unique<LogicDailyData>();
    daily_->decode(s);
    conf_ = std::make_unique<LogicConfData>();
    conf_->decode(s);
    id_ = LogicLong::decode(s);
    const i32 n = s.readVInt();
    notifications_.clear();
    for (i32 i = 0; i < n; ++i) {
        const i32 type = s.readVInt();
        auto notif = createNotificationByType(type);
        if (!notif) throw pending_reverse("notification type " + std::to_string(type));
        notif->decode(s);
        notifications_.emplace_back(type, std::move(notif));
    }
    v76_ = s.readVInt();
    b72_ = s.readBoolean();
    const i32 nd = s.readVInt();
    drops_.clear();
    dropsNull_ = (nd == 0); // ambiguous with empty; assume null
    for (i32 i = 0; i < nd; ++i) {
        auto d = std::make_unique<GatchaDrop>();
        d->decode(s);
        drops_.push_back(std::move(d));
    }
    const i32 nr = s.readVInt();
    refs_.clear();
    for (i32 i = 0; i < nr; ++i) {
        DataReference r;
        r.classId = s.readVInt();
        r.instanceId = s.readVInt();
        refs_.push_back(r);
    }
}

std::unique_ptr<Notification> LogicClientHome::removeNotification(i32 id) {
    for (auto it = notifications_.begin(); it != notifications_.end(); ++it) {
        if (it->second && it->second->v8_ == id) {
            auto removed = std::move(it->second);
            notifications_.erase(it);
            return removed;
        }
    }
    return nullptr;
}

} // namespace titan
