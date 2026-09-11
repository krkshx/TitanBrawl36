#pragma once

// BattlePlayerMap — reversed as LogicBattlePlayerMap
// (see game/battle/LogicBattlePlayerMap.cpp). Alias kept so existing users
// (EventData, StartLoadingMessage) don't churn.

#include "titan/game/battle/LogicBattlePlayerMap.cpp"

namespace titan {

using BattlePlayerMap = LogicBattlePlayerMap;

} // namespace titan
