#pragma once
#include "../../titan/core/ByteStream.cpp"
#include <cstdint>
#include <string>
#include <vector>

struct DataRef {
    std::int32_t type = 0;
    std::int32_t id = 0;
};

class LogicDailyData {
public:
    void encode(ByteStream &s) const {
        s.writeVInt(a0_);
        s.writeVInt(a1_);
        s.writeVInt(a2_);
        s.writeVInt(a3_);
        s.writeVInt(a4_);
        s.writeVInt(a5_);
        s.writeVInt(a6_);
        writeRef(s, r0_);
        writeRef(s, r1_);
        s.writeVInt(static_cast<std::int32_t>(ids_.size()));
        for (std::int32_t v : ids_) {
            s.writeVInt(v);
        }
        s.writeVInt(static_cast<std::int32_t>(refs_.size()));
        for (const DataRef &r : refs_) {
            writeRef(s, r);
        }
        s.writeVInt(cooldownSeconds_);
        s.writeVInt(brawlPassPoints_);
        s.writeVInt(starPoints_);
        s.writeVInt(eventPoints_);
        s.writeBoolean(bonusAvailable_);
        s.writeVInt(coins_);
        s.writeVInt(gems_);
        s.writeVInt(starpowerPoints_);
        s.writeVInt(tickets_);
    }
    void decode(ByteStream &s) {
        a0_ = s.readVInt();
        a1_ = s.readVInt();
        a2_ = s.readVInt();
        a3_ = s.readVInt();
        a4_ = s.readVInt();
        a5_ = s.readVInt();
        a6_ = s.readVInt();
        r0_ = readRef(s);
        r1_ = readRef(s);
        std::int32_t n = s.readVInt();
        ids_.resize(static_cast<std::size_t>(n));
        for (std::int32_t i = 0; i < n; i++) {
            ids_[static_cast<std::size_t>(i)] = s.readVInt();
        }
        std::int32_t m = s.readVInt();
        refs_.resize(static_cast<std::size_t>(m));
        for (std::int32_t i = 0; i < m; i++) {
            refs_[static_cast<std::size_t>(i)] = readRef(s);
        }
        cooldownSeconds_ = s.readVInt();
        brawlPassPoints_ = s.readVInt();
        starPoints_ = s.readVInt();
        eventPoints_ = s.readVInt();
        bonusAvailable_ = s.readBoolean();
        coins_ = s.readVInt();
        gems_ = s.readVInt();
        starpowerPoints_ = s.readVInt();
        tickets_ = s.readVInt();
    }
    std::int32_t a0_ = 0;
    std::int32_t a1_ = 0;
    std::int32_t a2_ = 0;
    std::int32_t a3_ = 0;
    std::int32_t a4_ = 0;
    std::int32_t a5_ = 0;
    std::int32_t a6_ = 0;
    DataRef r0_;
    DataRef r1_;
    std::vector<std::int32_t> ids_;
    std::vector<DataRef> refs_;
    std::int32_t cooldownSeconds_ = 0;
    std::int32_t brawlPassPoints_ = 0;
    std::int32_t starPoints_ = 0;
    std::int32_t eventPoints_ = 0;
    bool bonusAvailable_ = false;
    std::int32_t coins_ = 0;
    std::int32_t gems_ = 0;
    std::int32_t starpowerPoints_ = 0;
    std::int32_t tickets_ = 0;
private:
    static void writeRef(ByteStream &s, const DataRef &r) {
        s.writeVInt(r.type);
        s.writeVInt(r.id);
    }
    static DataRef readRef(ByteStream &s) {
        DataRef r;
        r.type = s.readVInt();
        r.id = s.readVInt();
        return r;
    }
};
