// Self-check for GlobalID math and the CSV data tables.
// Loads the real decompiled assets (pass assets dir as argv[1],
// or set TITAN_ASSETS, else tries ../assets and assets).

#include "titan/game/DataTables.hpp"

#include <cstdio>
#include <cstdlib>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

static std::string assetsDir(int argc, char** argv) {
    if (argc > 1) return argv[1];
    if (const char* e = std::getenv("TITAN_ASSETS")) return e;
    return "../assets";
}

int main(int argc, char** argv) {
    // GlobalID math (0x6a6b84 / 0x9188a0).
    CHECK(GlobalID::make(16, 0) == 16000000);
    CHECK(GlobalID::getClassID(16000003) == 16);
    CHECK(GlobalID::getInstanceID(16000003) == 3);

    // CSV dialect: quotes, empty fields, types row skipped.
    {
        const CsvTable t = parseCsv("A,B,C\nstring,int,boolean\na,,true\n\"x,y\",2,");
        CHECK(t.columns.size() == 3 && t.rows.size() == 2);
        CHECK(t.get(0, "A") == "a");
        CHECK(t.getInt(0, "B") == 0);
        CHECK(t.getBool(0, "C") == true);
        CHECK(t.get(1, "A") == "x,y");
        CHECK(t.getInt(1, "B") == 2);
    }

    DataTables dt;
    const int loaded = dt.load(assetsDir(argc, argv));
    std::printf("tables loaded: %d\n", loaded);
    CHECK(loaded >= 30);
    if (loaded == 0) {
        std::puts("datatables: ASSETS MISSING (skipped live checks)");
        return failures == 0 ? 0 : 1;
    }
    // characters.csv (class 16): Shelly is row 0 (ShotgunGirl).
    {
        const CsvTable* ch = dt.table(16);
        CHECK(ch != nullptr && !ch->rows.empty());
        CHECK(ch->get(0, "Name") == "ShotgunGirl");
        CHECK(ch->getInt(0, "Speed") == 720);
        CHECK(ch->getInt(0, "Hitpoints") == 3800);
    }
    // globals.csv (class 3): STARTING_DIAMONDS.
    {
        const CsvTable* g = dt.table(3);
        CHECK(g != nullptr);
        bool found = false;
        for (int i = 0; i < static_cast<int>(g->rows.size()); ++i) {
            if (g->get(i, "Name") == "STARTING_DIAMONDS") {
                CHECK(g->getInt(i, "NumberValue") == 0);
                found = true;
            }
        }
        CHECK(found);
    }
    // GlobalID-addressed lookup: Shelly == 16000000.
    CHECK(dt.get(16000000, "Name") == "ShotgunGirl");

    if (failures == 0) std::puts("datatables: all ok");
    return failures == 0 ? 0 : 1;
}
