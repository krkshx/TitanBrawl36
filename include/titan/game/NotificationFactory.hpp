#pragma once

// NotificationFactory::create @0x5f5d50: int type id -> notification.
// The binary switches on char-range ids ('?'=63 … '^'=94); most cases build
// a BaseNotification with a subclass vtable, a few call dedicated ctors.
// This port maps the types whose wire format is already reversed and
// returns nullptr for the rest (callers throw pending_reverse with the id).
// Add a case here with every newly reversed notification class.

#include "titan/game/BaseNotification.hpp"
#include "titan/game/FreeTextNotification.hpp"

#include <memory>

namespace titan {

inline std::unique_ptr<BaseNotification> createNotificationByType(int type) {
    switch (type) {
        case 81: return std::make_unique<FreeTextNotification>();
        default: return nullptr;
    }
}

} // namespace titan
