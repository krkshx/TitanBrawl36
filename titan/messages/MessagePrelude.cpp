#pragma once

// Common prelude for every per-message header: core types, shared bases,
// already-reversed game entries. Pending nested entries are included
// per-file (only the ones each message references).

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/core/PiranhaMessage.cpp"
#include "titan/game/alliance/AllianceFullEntry.cpp"
#include "titan/game/alliance/AllianceHeaderEntry.cpp"
#include "titan/game/alliance/AllianceMemberEntry.cpp"
#include "titan/game/battle/ClientInput.cpp"
#include "titan/game/avatar/LogicClientAvatar.cpp"
#include "titan/game/util/LogicCompressedString.cpp"
#include "titan/game/data/LogicDataSlot.cpp"
#include "titan/game/avatar/PlayerDisplayData.cpp"
#include "titan/messages/Common.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
