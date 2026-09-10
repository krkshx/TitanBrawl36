#pragma once

// FriendOnlineStatus — the binary's class is FriendOnlineStatusEntry
// (encode @0x1a39e8, decode @0x26a79c); alias kept so existing users
// (FriendOnlineStatusEntryMessage) don't churn.

#include "titan/game/FriendOnlineStatusEntry.hpp"

namespace titan {

using FriendOnlineStatus = FriendOnlineStatusEntry;

} // namespace titan
