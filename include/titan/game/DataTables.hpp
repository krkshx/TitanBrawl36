#pragma once

// DataTables — CSV-backed game data, mirroring LogicDataTables.
//
// Table map: LogicResources::createDataTableResourcesArray @0x421b04
// (see data/datatable_map.csv). CSV dialect (Supercell): row 0 = column
// names, row 1 = column types, rows 2+ = data; comma-separated with
// double-quoted fields. instanceId = data-row index (0-based), so
// GlobalID = classId * 1000000 + row.

#include "titan/game/GlobalID.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace titan {

struct CsvTable {
    std::vector<std::string> columns;
    std::vector<std::string> types;
    std::vector<std::vector<std::string>> rows; // rows[i] == instance i

    [[nodiscard]] int columnIndex(const std::string& name) const;
    [[nodiscard]] std::string get(int row, const std::string& col,
                                  const std::string& fallback = "") const;
    [[nodiscard]] int getInt(int row, const std::string& col, int fallback = 0) const;
    [[nodiscard]] bool getBool(int row, const std::string& col, bool fallback = false) const;
};

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

private:
    struct Entry {
        int classId = -1;
        CsvTable table;
    };
    std::vector<Entry> entries_;
};

// Standalone CSV dialect parser (also unit-tested).
CsvTable parseCsv(const std::string& text);

} // namespace titan
