#pragma once

// BattleLogEntry::encode @0x8e4504 (decode via stream-ctor @0x2e6380).
// Wire: vint x5 (+0/+36/+4/+8/+12), bool +16, dataref +24, vint +32/+40,
// int64 +48/+56, vint +64, bool +68, vint count + BattleLogPlayerEntry loop,
// vint +72, bool +76, vint +80, bool +84, vint +88,
// optional +112 (bool + logiclong + vint + stringref + logiclong),
// optional +152 (bool + LogicUuid + bool + vint + vint; absent -> final vint 0).
// BattleLogPlayerEntry/LogicUuid stay pending. Split out of the entry wave.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/BattleLogPlayerEntry.hpp"
#include "titan/game/LogicUuid.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class BattleLogEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    i32 v0_ = 0, v36_ = 0, v4_ = 0, v8_ = 0, v12_ = 0;
    bool b16_ = false;
    std::optional<DataReference> ref_;
    i32 v32_ = 0, v40_ = 0;
    i64 q48_ = 0, q56_ = 0;
    i32 v64_ = 0;
    bool b68_ = false;
    std::vector<std::unique_ptr<BattleLogPlayerEntry>> players_;
    i32 v72_ = 0;
    bool b76_ = false;
    i32 v80_ = 0;
    bool b84_ = false;
    i32 v88_ = 0;
    bool hasOpt112_ = false;
    LogicLong id112_, id144_;
    i32 v120_ = 0;
    std::string ref128_;
    bool hasOpt152_ = false;
    std::unique_ptr<LogicUuid> uuid_;
    bool b160_ = false;
    i32 v164_ = 0, v168_ = 0;
};

} // namespace titan
