// DataTables loader — split out of the old DataTables.cpp
// (CsvTable lives in CsvTable.cpp now).

#include "titan/game/DataTables.hpp"

#include <fstream>
#include <sstream>

namespace titan {

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
