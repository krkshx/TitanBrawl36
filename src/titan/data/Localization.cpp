#pragma once
#include "CsvTable.cpp"
#include <cstdlib>
#include <string>

class Localization {
public:
    bool load(const std::string &dir) {
        std::string lang = "en";
        const char *env = std::getenv("LANG");
        if (env && env[0] && env[1]) {
            lang = std::string(env, 2);
        }
        if (!table_.load(dir + "/localization/" + lang + ".csv")) {
            if (!table_.load(dir + "/localization/texts.csv")) {
                return false;
            }
        }
        if (table_.cols() > 1) {
            colName_ = table_.header(1);
        }
        return table_.rows() > 0;
    }
    std::string text(const std::string &tid, const std::string &fallback) const {
        for (std::size_t r = 0; r < table_.rows(); r++) {
            if (table_.get(r, "TID") == tid) {
                std::string v = table_.get(r, colName_);
                if (!v.empty()) {
                    return v;
                }
                break;
            }
        }
        return fallback;
    }
private:
    CsvTable table_;
    std::string colName_ = "EN";
};
