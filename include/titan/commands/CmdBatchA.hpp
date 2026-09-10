#pragma once

// Command batch A (types 201-224) — reversed from libg_decrypted.so.
// Each body = LogicCommand::encode base prefix + own fields below.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(DeliveryUnit);
TITAN_PENDING_ENTRY(ForcedDrops);
TITAN_PENDING_ENTRY(LogicConfData);
TITAN_PENDING_ENTRY(TimedOffer);
TITAN_PENDING_ENTRY(LogicOfferBundle);
TITAN_PENDING_ENTRY(LogicQuests);
TITAN_PENDING_ENTRY(LogicPlayerRankedSeasonData);
TITAN_PENDING_ENTRY(CooldownEntry);

// ---- 201 LogicChangeAvatarNameCommand: stringref + vint ----
class LogicChangeAvatarNameCommand : public LogicCommand {
public:
    int getCommandType() const override { return 201; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeStringReference(name_);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        name_ = s.readStringReference();
        v_ = s.readVInt();
    }
    std::string name_;
    i32 v_ = 0;
};

// ---- 202 LogicDiamondsAddedCommand: bool + int x3 + string ----
class LogicDiamondsAddedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 202; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(b_);
        s.writeInt(i1_);
        s.writeInt(i2_);
        s.writeInt(i3_);
        s.writeString(str_ ? &*str_ : nullptr);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        b_ = s.readBoolean();
        i1_ = s.readInt();
        i2_ = s.readInt();
        i3_ = s.readInt();
        str_ = s.readString();
    }
    bool b_ = false;
    i32 i1_ = 0, i2_ = 0, i3_ = 0;
    std::optional<std::string> str_;
};

// ---- 203 LogicGiveDeliveryItemsCommand ----
class LogicGiveDeliveryItemsCommand : public LogicCommand {
public:
    int getCommandType() const override { return 203; }
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    i32 v1_ = 0;
    std::vector<std::unique_ptr<DeliveryUnit>> units_;
    bool hasDrops_ = false;
    std::unique_ptr<ForcedDrops> drops_;
    i32 v2_ = 0, v3_ = 0, v4_ = 0;
    bool b1_ = false, b2_ = false;
};

// ---- 204 LogicDayChangedCommand: [LogicConfData] ----
class LogicDayChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 204; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(static_cast<bool>(conf_));
        if (conf_) conf_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        if (s.readBoolean()) {
            conf_ = std::make_unique<LogicConfData>();
            conf_->decode(s);
        }
    }
    std::unique_ptr<LogicConfData> conf_;
};

// ---- 205 LogicServerCommand: vint ----
class LogicServerCommand : public LogicCommand {
public:
    int getCommandType() const override { return 205; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
    }
    i32 v_ = 0;
};

// ---- 206 LogicAddNotificationCommand: [vint type] + [object] ----
class LogicAddNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 206; }
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    bool hasNested_ = false;
    i32 type_ = 0;
    std::unique_ptr<NestedEntry> object_; // concrete class pending (virtual-encode)
};

// ---- 207 LogicChangeResourcesCommand: vint + vint ----
class LogicChangeResourcesCommand : public LogicCommand {
public:
    int getCommandType() const override { return 207; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 208 LogicTransactionsRevokedCommand: int ----
class LogicTransactionsRevokedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 208; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readInt();
    }
    i32 v_ = 0;
};

// ---- 209 LogicKeyPoolChangedCommand: vint + vint ----
class LogicKeyPoolChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 209; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 210 LogicIAPChangedCommand: vint + [TimedOffer] ----
class LogicIAPChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 210; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        s.writeBoolean(static_cast<bool>(offer_));
        if (offer_) offer_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        if (s.readBoolean()) {
            offer_ = std::make_unique<TimedOffer>();
            offer_->decode(s);
        }
    }
    i32 v_ = 0;
    std::unique_ptr<TimedOffer> offer_;
};

// ---- 211 LogicOffersChangedCommand: count(0-if-null) + bundles ----
class LogicOffersChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 211; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(isNull_ ? 0 : static_cast<i32>(bundles_.size()));
        for (const auto& b : bundles_) b->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        const i32 n = s.readVInt();
        bundles_.clear();
        isNull_ = (n == 0); // 0 is ambiguous (empty or null); assume null
        for (i32 i = 0; i < n; ++i) {
            auto b = std::make_unique<LogicOfferBundle>();
            b->decode(s);
            bundles_.push_back(std::move(b));
        }
    }
    std::vector<std::unique_ptr<LogicOfferBundle>> bundles_;
    bool isNull_ = false;
};

// ---- 212 LogicPlayerDataChangedCommand: vint ----
class LogicPlayerDataChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 212; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
    }
    i32 v_ = 0;
};

// ---- 213 LogicInviteBlockingChangedCommand: bool ----
class LogicInviteBlockingChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 213; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(b_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        b_ = s.readBoolean();
    }
    bool b_ = false;
};

// ---- 214 LogicGemNameChangeStateChangedCommand: vint + vint ----
class LogicGemNameChangeStateChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 214; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 215 LogicSetSupportedCreatorCommand: [string] ----
class LogicSetSupportedCreatorCommand : public LogicCommand {
public:
    int getCommandType() const override { return 215; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(hasStr_);
        if (hasStr_) s.writeString(str_ ? &*str_ : nullptr);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        hasStr_ = s.readBoolean();
        if (hasStr_) str_ = s.readString();
    }
    bool hasStr_ = false;
    std::optional<std::string> str_;
};

// ---- 216 LogicCooldownExpiredCommand: vint + dataref ----
class LogicCooldownExpiredCommand : public LogicCommand {
public:
    int getCommandType() const override { return 216; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        DataReference::encodeNullable(s, ref_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
    }
    i32 v_ = 0;
    std::optional<DataReference> ref_;
};

// ---- 217 LogicProLeagueSeasonChangedCommand: vint + vint ----
class LogicProLeagueSeasonChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 217; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 218 LogicBrawlPassSeasonChangedCommand: vint + vint ----
class LogicBrawlPassSeasonChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 218; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 219 LogicBrawlPassUnlockedCommand: vint ----
class LogicBrawlPassUnlockedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 219; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
    }
    i32 v_ = 0;
};

// ---- 220 LogicHeroWinQuestsChangedCommand: [LogicQuests] ----
class LogicHeroWinQuestsChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 220; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(static_cast<bool>(quests_));
        if (quests_) quests_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        if (s.readBoolean()) {
            quests_ = std::make_unique<LogicQuests>();
            quests_->decode(s);
        }
    }
    std::unique_ptr<LogicQuests> quests_;
};

// ---- 221 LogicTeamChatMuteStateChangedCommand: bool ----
class LogicTeamChatMuteStateChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 221; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(b_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        b_ = s.readBoolean();
    }
    bool b_ = false;
};

// ---- 222 LogicRankedSeasonChangedCommand: [LogicPlayerRankedSeasonData] ----
class LogicRankedSeasonChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 222; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(static_cast<bool>(season_));
        if (season_) season_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        if (s.readBoolean()) {
            season_ = std::make_unique<LogicPlayerRankedSeasonData>();
            season_->decode(s);
        }
    }
    std::unique_ptr<LogicPlayerRankedSeasonData> season_;
};

// ---- 223 LogicCooldownAddedCommand: count(-1-if-null) + entries ----
class LogicCooldownAddedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 223; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        if (isNull_) {
            s.writeVInt(-1);
            return;
        }
        s.writeVInt(static_cast<i32>(entries_.size()));
        for (const auto& e : entries_) e->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        const i32 n = s.readVInt();
        entries_.clear();
        isNull_ = (n < 0);
        for (i32 i = 0; i < n; ++i) {
            auto e = std::make_unique<CooldownEntry>();
            e->decode(s);
            entries_.push_back(std::move(e));
        }
    }
    std::vector<std::unique_ptr<CooldownEntry>> entries_;
    bool isNull_ = false;
};

// ---- 224 LogicSetESportsHubNotificationCommand: int ----
class LogicSetESportsHubNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 224; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readInt();
    }
    i32 v_ = 0;
};

} // namespace titan
