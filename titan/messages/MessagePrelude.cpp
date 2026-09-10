#pragma once

// Common prelude for every per-message header: core types, shared bases,
// already-reversed game entries. Pending nested entries are included
// per-file (only the ones each message references).

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/game/AllianceFullEntry.hpp"
#include "titan/game/AllianceHeaderEntry.hpp"
#include "titan/game/AllianceMemberEntry.hpp"
#include "titan/game/ClientInput.hpp"
#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicCompressedString.hpp"
#include "titan/game/LogicDataSlot.hpp"
#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Common.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
