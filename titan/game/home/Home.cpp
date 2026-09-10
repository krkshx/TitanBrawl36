#pragma once

// SCC-merge exception to the one-class-per-file rule.
//
// GatchaDrop <-> LogicClientHome <-> LogicHomeMode form a strongly
// connected component at the type level (verified by tools analysis):
//   - LogicClientHome owns std::vector<std::unique_ptr<GatchaDrop>>
//     and make_unique<GatchaDrop>() in decode, so it needs GatchaDrop
//     complete;
//   - GatchaDrop::doDrop @0x4083c4 calls LogicHomeMode::getPlayerAvatar /
//     getHome, LogicClientHome members (daily_) and LogicClientAvatar
//     methods, so it needs all three complete;
//   - LogicHomeMode owns std::unique_ptr<LogicClientHome>.
// No include order satisfies all three in separate TUs (proved by build
// failure); the three classes therefore share this single TU, ordered
// GatchaDrop -> LogicClientHome -> LogicHomeMode (each complete before
// the next needs it), bodies last. External deps stay as includes.

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/data/LogicConfData.cpp"
#include "titan/game/player/LogicDailyData.cpp"
#include "titan/messages/pending/Notification.cpp"
#include "titan/game/notif/NotificationFactory.cpp"
#include "titan/game/data/DataTables.cpp"
#include "titan/game/avatar/LogicClientAvatar.cpp"
#include "titan/game/data/LogicData.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

// Forward declarations (SCC members reference each other).
class GatchaDrop;
class LogicClientHome;
class LogicHomeMode;

// GatchaDrop::encode @0x7509c0.
// Wire: vint @+4, dataref (nullable @+8), vint @+0, dataref @+16/@+24/@+32
// (nullable), vint @+40, vint @+44.
// Decode mirrors the same order (stream ctor used by LogicClientHome::decode).
//
// doDrop @0x4083c4 dispatches on v0_ (the +0 type field): cases 2/3/8 are
// implemented (player-data ints, diamonds); 1/4/6/7/9/0xB/0xC need hero
// unlock/power/item/skin tables and throw pending_reverse; unknown types
// are a no-op like the binary's default. Listener callbacks skipped.
class GatchaDrop : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v4_);
        DataReference::encodeNullable(s, ref8_);
        s.writeVInt(v0_);
        DataReference::encodeNullable(s, ref16_);
        DataReference::encodeNullable(s, ref24_);
        DataReference::encodeNullable(s, ref32_);
        s.writeVInt(v40_);
        s.writeVInt(v44_);
    }
    void decode(ByteStream& s) override {
        v4_ = s.readVInt();
        ref8_ = DataReference::decodeNullable(s);
        v0_ = s.readVInt();
        ref16_ = DataReference::decodeNullable(s);
        ref24_ = DataReference::decodeNullable(s);
        ref32_ = DataReference::decodeNullable(s);
        v40_ = s.readVInt();
        v44_ = s.readVInt();
    }

    i32 v4_ = 0;                          // +4 (wired first)
    std::optional<DataReference> ref8_;   // +8
    i32 v0_ = 0;                          // +0 (drop type, doDrop switches on it)
    std::optional<DataReference> ref16_;  // +16
    std::optional<DataReference> ref24_;  // +24
    std::optional<DataReference> ref32_;  // +32
    i32 v40_ = 0, v44_ = 0;               // +40, +44

    // doDrop @0x4083c4 (out-of-line: needs complete HomeMode).
    void doDrop(LogicHomeMode* home, bool flag);
};

// LogicClientHome::encode @0x485314, decode @0x3b1044.
// Wire: LogicDailyData, LogicConfData, logiclong, vint count +
// (vint type + Notification) loop, vint, bool, optional GatchaDrop vector,
// vint count + dataref loop. Nested data classes stay pending.
class LogicClientHome : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    // removeNotification @0x8f73e4: removes the first entry whose base
    // +8 int (v8_) equals id, transferring ownership (null if absent).
    std::unique_ptr<Notification> removeNotification(i32 id);
    std::unique_ptr<LogicDailyData> daily_;
    std::unique_ptr<LogicConfData> conf_;
    LogicLong id_;
    std::vector<std::pair<i32, std::unique_ptr<Notification>>> notifications_;
    i32 v76_ = 0;
    bool b72_ = false;
    std::vector<std::unique_ptr<GatchaDrop>> drops_;
    bool dropsNull_ = false; // null -> vint 0 on the wire
    std::vector<DataReference> refs_;
};

// LogicHomeMode — home game state root (minimal port).
// Only the surface needed by command execute() so far:
//   getPlayerAvatar @0x5543f0 = *(this+40),
//   getHome @0x598c2c = *(this+32).
// UI listener hooks (getGameListener @0x3a6658) are platform code and stay
// out; execute() implementations skip listener callbacks explicitly.
class LogicHomeMode {
public:
    // getPlayerAvatar @0x5543f0.
    [[nodiscard]] LogicClientAvatar* getPlayerAvatar() const {
        return avatar_.get();
    }
    void setPlayerAvatar(std::unique_ptr<LogicClientAvatar> avatar) {
        avatar_ = std::move(avatar);
    }
    // getHome @0x598c2c.
    [[nodiscard]] LogicClientHome* getHome() const { return home_.get(); }
    void setHome(std::unique_ptr<LogicClientHome> home) {
        home_ = std::move(home);
    }
    // Data tables (CSV game data). Not in the binary's HomeMode surface
    // used so far — port-owned wiring so executes can resolve materials
    // (e.g. gold = resources row 1) without globals.
    void setDataTables(const DataTables* tables) { tables_ = tables; }
    [[nodiscard]] const DataTables* dataTables() const { return tables_; }
    // Well-known material rows (see csv_logic/resources.csv).
    [[nodiscard]] LogicData goldData() const { return LogicData(tables_, 5, 1); }

private:
    std::unique_ptr<LogicClientHome> home_; // +32
    std::unique_ptr<LogicClientAvatar> avatar_; // +40
    const DataTables* tables_ = nullptr; // port-owned
};

} // namespace titan

namespace titan {

// GatchaDrop::doDrop @0x4083c4.
inline void GatchaDrop::doDrop(LogicHomeMode* home, bool flag) {
    LogicClientAvatar* avatar = home ? home->getPlayerAvatar() : nullptr;
    if (!avatar) return;
    switch (v0_) {
        case 2: { // playerData +172 += amount
            LogicClientHome* h = home->getHome();
            if (!h || !h->daily_) return;
            h->daily_->tail43_[0] += v4_;
            return;
        }
        case 3: { // playerData +280 += amount
            LogicClientHome* h = home->getHome();
            if (!h || !h->daily_) return;
            h->daily_->v70_ += v4_;
            return;
        }
        case 8: { // diamonds (paid iff flag; listener arg dropped)
            if (flag) {
                avatar->addPaidDiamonds(v4_);
            } else {
                avatar->addFreeDiamonds(v4_);
            }
            return;
        }
        case 1: // unlockHero (needs card tables)
        case 4: // setItem + addNewItem (needs card tables)
        case 6: // increaseHeroPower (needs hero tables)
        case 7: // material via DataTables global (unresolvable statically)
        case 9: // addUnlockedSkin (needs skin tables)
        case 0xB: // vanity add (needs emote tables)
        case 0xC: // star-points material (needs DataTables global)
            throw pending_reverse("GatchaDrop::doDrop: type needs data tables");
        default:
            return; // binary default: no-op
    }
}

inline void LogicClientHome::encode(ByteStream& s) const {
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

inline void LogicClientHome::decode(ByteStream& s) {
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

inline std::unique_ptr<Notification> LogicClientHome::removeNotification(i32 id) {
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
