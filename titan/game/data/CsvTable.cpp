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

// CsvTable CSV dialect parser — split out of DataTables.cpp.


#include <sstream>

namespace titan {

namespace {

std::vector<std::string> splitLine(const std::string& line) {
    // Comma-separated with double-quoted fields; "" inside quotes = quote.
    std::vector<std::string> out;
    std::string cur;
    bool inQuotes = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    cur += '"';
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                cur += c;
            }
        } else if (c == '"') {
            inQuotes = true;
        } else if (c == ',') {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    out.push_back(cur);
    return out;
}

} // namespace

CsvTable parseCsv(const std::string& text) {
    CsvTable t;
    std::istringstream in(text);
    std::string line;
    bool first = true, second = true;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        auto fields = splitLine(line);
        if (first) {
            t.columns = std::move(fields);
            first = false;
        } else if (second) {
            t.types = std::move(fields);
            second = false;
        } else {
            t.rows.push_back(std::move(fields));
        }
    }
    return t;
}

inline int CsvTable::columnIndex(const std::string& name) const {
    for (std::size_t i = 0; i < columns.size(); ++i) {
        if (columns[i] == name) return static_cast<int>(i);
    }
    return -1;
}

inline std::string CsvTable::get(int row, const std::string& col,
                          const std::string& fallback) const {
    if (row < 0 || row >= static_cast<int>(rows.size())) return fallback;
    const int c = columnIndex(col);
    if (c < 0 || c >= static_cast<int>(rows[static_cast<std::size_t>(row)].size())) {
        return fallback;
    }
    return rows[static_cast<std::size_t>(row)][static_cast<std::size_t>(c)];
}

inline int CsvTable::getInt(int row, const std::string& col, int fallback) const {
    const std::string v = get(row, col);
    if (v.empty()) return fallback;
    try {
        return std::stoi(v);
    } catch (...) {
        return fallback;
    }
}

inline bool CsvTable::getBool(int row, const std::string& col, bool fallback) const {
    const std::string v = get(row, col);
    if (v == "true" || v == "TRUE" || v == "1") return true;
    if (v == "false" || v == "FALSE" || v == "0" || v.empty()) {
        return v.empty() ? fallback : false;
    }
    return fallback;
}

} // namespace titan
