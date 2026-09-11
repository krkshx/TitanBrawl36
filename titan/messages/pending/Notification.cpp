#pragma once

// Notification — the binary's base is BaseNotification
// (encode @0x7beae0, decode @0x605478); alias kept so existing users
// (LogicClientHome) don't churn. Concrete types come from
// game/notif/NotificationFactory.cpp.

#include "titan/game/notif/BaseNotification.cpp"

namespace titan {

using Notification = BaseNotification;

} // namespace titan
