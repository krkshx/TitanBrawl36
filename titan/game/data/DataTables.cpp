#pragma once

// DataTables — CSV-backed game data, mirroring LogicDataTables.
//
// Table map: LogicResources::createDataTableResourcesArray @0x421b04
// (see data/datatable_map.csv). instanceId = data-row index (0-based), so
// GlobalID = classId * 1000000 + row.
// Split out of the old DataTables.hpp (CsvTable lives in CsvTable.hpp).

#include "titan/game/data/CsvTable.cpp"
#include "titan/game/util/GlobalID.cpp"

#include <string>
#include <vector>

namespace titan {

class DataTables {
public:
    // Loads every table in data/datatable_map.csv under assetsDir.
    // Missing files are skipped (returns count of loaded tables).
    int load(const std::string& assetsDir, const std::string& mapFile = "data/datatable_map.csv");

    [[nodiscard]] const CsvTable* table(int classId) const;
    [[nodiscard]] int tableCount() const { return static_cast<int>(entries_.size()); }

    // Row access by GlobalID (instanceId = row index).
    [[nodiscard]] std::string get(int globalId, const std::string& col,
                                  const std::string& fallback = "") const;

    // Display name of a DataReference target ("Name" column, "" if unknown).
    // Used by the viewer to show hero/item names instead of bare ids.
    [[nodiscard]] std::string getName(int classId, int row) const {
        return get(GlobalID::make(classId, row), "Name", "");
    }

private:
    struct Entry {
        int classId = -1;
        CsvTable table;
    };
    std::vector<Entry> entries_;
};

} // namespace titan

// DataTables loader — split out of the old DataTables.cpp
// (CsvTable lives in CsvTable.cpp now).


#include <fstream>
#include <sstream>

namespace titan {

inline int DataTables::load(const std::string& assetsDir, const std::string& mapFile) {
    // mapFile is repo-relative (repo root); fall back to CWD-joined path.
    std::ifstream map(mapFile);
    if (!map.is_open()) {
        map = std::ifstream(std::string("../") + mapFile);
        if (!map.is_open()) return 0;
    }
    int loaded = 0;
    std::string line;
    while (std::getline(map, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ls(line);
        std::string idStr, path, flag;
        if (!std::getline(ls, idStr, ',') || !std::getline(ls, path, ',')) continue;
        const int classId = std::stoi(idStr);
        if (path == "EMPTY") continue;
        std::ifstream f(assetsDir + "/" + path);
        if (!f.is_open()) continue;
        std::ostringstream ss;
        ss << f.rdbuf();
        entries_.push_back({classId, parseCsv(ss.str())});
        ++loaded;
    }
    return loaded;
}

inline const CsvTable* DataTables::table(int classId) const {
    for (const auto& e : entries_) {
        if (e.classId == classId) return &e.table;
    }
    return nullptr;
}

inline std::string DataTables::get(int globalId, const std::string& col,
                            const std::string& fallback) const {
    const CsvTable* t = table(GlobalID::getClassID(globalId));
    if (t == nullptr) return fallback;
    return t->get(GlobalID::getInstanceID(globalId), col, fallback);
}

} // namespace titan
