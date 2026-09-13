#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class CsvTable {
public:
    bool load(const std::string &path) {
        std::ifstream f(path);
        if (!f) {
            return false;
        }
        rows_.clear();
        headers_.clear();
        std::string line;
        bool first = true;
        while (std::getline(f, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            std::vector<std::string> cells = split(line);
            if (first) {
                headers_ = cells;
                first = false;
            } else {
                rows_.push_back(cells);
            }
        }
        return true;
    }
    std::size_t rows() const { return rows_.size(); }
    std::size_t cols() const { return headers_.size(); }
    std::string header(std::size_t i) const {
        if (i < headers_.size()) {
            return headers_[i];
        }
        return std::string();
    }
    std::string get(std::size_t r, const std::string &col) const {
        for (std::size_t i = 0; i < headers_.size(); i++) {
            if (headers_[i] == col) {
                if (r < rows_.size() && i < rows_[r].size()) {
                    return rows_[r][i];
                }
            }
        }
        return std::string();
    }
private:
    static std::vector<std::string> split(const std::string &line) {
        std::vector<std::string> out;
        std::string cur;
        bool quoted = false;
        for (char c : line) {
            if (c == '"') {
                quoted = !quoted;
            } else if (c == ',' && !quoted) {
                out.push_back(cur);
                cur.clear();
            } else {
                cur.push_back(c);
            }
        }
        out.push_back(cur);
        return out;
    }
    std::vector<std::string> headers_;
    std::vector<std::vector<std::string>> rows_;
};
