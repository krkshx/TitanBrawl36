// DataTables CSV loader.

#include "titan/game/DataTables.hpp"

#include <fstream>
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

int CsvTable::columnIndex(const std::string& name) const {
    for (std::size_t i = 0; i < columns.size(); ++i) {
        if (columns[i] == name) return static_cast<int>(i);
    }
    return -1;
}

std::string CsvTable::get(int row, const std::string& col,
                          const std::string& fallback) const {
    if (row < 0 || row >= static_cast<int>(rows.size())) return fallback;
    const int c = columnIndex(col);
    if (c < 0 || c >= static_cast<int>(rows[static_cast<std::size_t>(row)].size())) {
        return fallback;
    }
    return rows[static_cast<std::size_t>(row)][static_cast<std::size_t>(c)];
}

int CsvTable::getInt(int row, const std::string& col, int fallback) const {
    const std::string v = get(row, col);
    if (v.empty()) return fallback;
    try {
        return std::stoi(v);
    } catch (...) {
        return fallback;
    }
}

bool CsvTable::getBool(int row, const std::string& col, bool fallback) const {
    const std::string v = get(row, col);
    if (v == "true" || v == "TRUE" || v == "1") return true;
    if (v == "false" || v == "FALSE" || v == "0" || v.empty()) {
        return v.empty() ? fallback : false;
    }
    return fallback;
}

int DataTables::load(const std::string& assetsDir, const std::string& mapFile) {
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

const CsvTable* DataTables::table(int classId) const {
    for (const auto& e : entries_) {
        if (e.classId == classId) return &e.table;
    }
    return nullptr;
}

std::string DataTables::get(int globalId, const std::string& col,
                            const std::string& fallback) const {
    const CsvTable* t = table(GlobalID::getClassID(globalId));
    if (t == nullptr) return fallback;
    return t->get(GlobalID::getInstanceID(globalId), col, fallback);
}

} // namespace titan
