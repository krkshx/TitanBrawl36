#pragma once

// BattlePlayerMap — reversed as LogicBattlePlayerMap
// (see game/LogicBattlePlayerMap.hpp). Alias kept so existing users
// (EventData, StartLoadingMessage) don't churn.

#include "titan/game/LogicBattlePlayerMap.hpp"

namespace titan {

using BattlePlayerMap = LogicBattlePlayerMap;

} // namespace titan
