#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class FileSystem {
public:
    static bool exists(const std::string &path) {
        std::ifstream f(path, std::ios::binary);
        return static_cast<bool>(f);
    }
    static std::vector<char> readAll(const std::string &path) {
        std::ifstream f(path, std::ios::binary);
        if (!f) {
            return {};
        }
        return std::vector<char>(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    }
    static bool writeAll(const std::string &path, const char *data, std::size_t n) {
        std::ofstream f(path, std::ios::binary);
        if (!f) {
            return false;
        }
        f.write(data, static_cast<std::streamsize>(n));
        return static_cast<bool>(f);
    }
    static std::string join(const std::string &a, const std::string &b) {
        if (a.empty()) {
            return b;
        }
        char last = a[a.size() - 1];
        if (last == '/' || last == '\\') {
            return a + b;
        }
#if defined(_WIN32)
        return a + "\\" + b;
#else
        return a + "/" + b;
#endif
    }
};
