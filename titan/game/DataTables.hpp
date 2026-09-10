#pragma once

// DataTables — CSV-backed game data, mirroring LogicDataTables.
//
// Table map: LogicResources::createDataTableResourcesArray @0x421b04
// (see data/datatable_map.csv). instanceId = data-row index (0-based), so
// GlobalID = classId * 1000000 + row.
// Split out of the old DataTables.hpp (CsvTable lives in CsvTable.hpp).

#include "titan/game/CsvTable.hpp"
#include "titan/game/GlobalID.hpp"

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
