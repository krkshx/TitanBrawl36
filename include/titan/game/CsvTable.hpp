#pragma once

// CsvTable — Supercell CSV dialect container.
// Row 0 of the file = column names, row 1 = column types, rows 2+ = data;
// comma-separated with double-quoted fields. rows[i] == instance i.
// Split out of DataTables.hpp; behavior unchanged.

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

// Standalone CSV dialect parser (also unit-tested).
CsvTable parseCsv(const std::string& text);

} // namespace titan
