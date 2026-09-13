#pragma once
#include <cstdio>
#include <string>
#include <vector>

class NativeFont {
public:
    static std::string osName() {
#if defined(_WIN32)
        return "windows";
#elif defined(__APPLE__)
        return "macos";
#elif defined(__linux__)
        return "linux";
#else
        return "unknown";
#endif
    }
    static bool exists(const std::string &path) {
        FILE *handle = fopen(path.c_str(), "rb");
        if (!handle) {
            return false;
        }
        fclose(handle);
        return true;
    }
    static std::string lower(std::string value) {
        for (std::size_t i = 0; i < value.size(); i++) {
            if (value[i] >= 'A' && value[i] <= 'Z') {
                value[i] = static_cast<char>(value[i] + 32);
            }
        }
        return value;
    }
    static bool isSystemFamily(const std::string &family) {
        std::string name = lower(family);
        return name.find("times") != std::string::npos || name.find("arial") != std::string::npos || name.find("helvetica") != std::string::npos || name.find("verdana") != std::string::npos || name.find("tahoma") != std::string::npos || name.find("serif") != std::string::npos || name.find("georgia") != std::string::npos;
    }
    static std::vector<std::string> candidatesFor(const std::string &family, bool bold) {
        std::string name = lower(family);
        bool serif = name.find("times") != std::string::npos || name.find("serif") != std::string::npos || name.find("roman") != std::string::npos || name.find("georgia") != std::string::npos;
        std::vector<std::string> result;
        if (serif) {
            appendSerif(result, bold);
        } else {
            appendSans(result, bold);
        }
        return result;
    }
    static std::vector<std::string> systemFallbacks() {
        std::vector<std::string> result;
#if defined(_WIN32)
        appendExisting(result, "C:/Windows/Fonts/arial.ttf");
        appendExisting(result, "C:/Windows/Fonts/times.ttf");
        appendExisting(result, "C:/Windows/Fonts/segoeui.ttf");
#elif defined(__APPLE__)
        appendExisting(result, "/System/Library/Fonts/Helvetica.ttc");
        appendExisting(result, "/System/Library/Fonts/Times.ttc");
        appendExisting(result, "/Library/Fonts/Arial.ttf");
        appendExisting(result, "/Library/Fonts/Times New Roman.ttf");
#else
        appendExisting(result, "/usr/share/fonts/TTF/DejaVuSans.ttf");
        appendExisting(result, "/usr/share/fonts/noto/NotoSans-Regular.ttf");
        appendExisting(result, "/usr/share/fonts/TTF/DejaVuSerif.ttf");
        appendExisting(result, "/usr/share/fonts/noto/NotoSerif-Regular.ttf");
        appendExisting(result, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        appendExisting(result, "/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
        appendExisting(result, "/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");
        appendExisting(result, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
        appendExisting(result, "/usr/share/fonts/truetype/liberation/LiberationSerif-Regular.ttf");
#endif
        return result;
    }
private:
    static void appendExisting(std::vector<std::string> &out, const std::string &path) {
        if (exists(path)) {
            out.push_back(path);
        }
    }
    static void appendSerif(std::vector<std::string> &out, bool bold) {
#if defined(_WIN32)
        if (bold) {
            appendExisting(out, "C:/Windows/Fonts/timesbd.ttf");
        }
        appendExisting(out, "C:/Windows/Fonts/times.ttf");
        appendExisting(out, "C:/Windows/Fonts/georgia.ttf");
#elif defined(__APPLE__)
        appendExisting(out, "/System/Library/Fonts/Times.ttc");
        appendExisting(out, "/Library/Fonts/Times New Roman.ttf");
        appendExisting(out, "/System/Library/Fonts/Georgia.ttf");
#else
        if (bold) {
            appendExisting(out, "/usr/share/fonts/TTF/DejaVuSerif-Bold.ttf");
            appendExisting(out, "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf");
            appendExisting(out, "/usr/share/fonts/noto/NotoSerif-Bold.ttf");
        }
        appendExisting(out, "/usr/share/fonts/TTF/DejaVuSerif.ttf");
        appendExisting(out, "/usr/share/fonts/noto/NotoSerif-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
        appendExisting(out, "/usr/share/fonts/truetype/liberation/LiberationSerif-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/TTF/LiberationSerif-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/gsfonts/NimbusRoman-Regular.otf");
        if (!bold) {
            appendExisting(out, "/usr/share/fonts/TTF/DejaVuSerif-Bold.ttf");
        }
#endif
    }
    static void appendSans(std::vector<std::string> &out, bool bold) {
#if defined(_WIN32)
        if (bold) {
            appendExisting(out, "C:/Windows/Fonts/arialbd.ttf");
        }
        appendExisting(out, "C:/Windows/Fonts/arial.ttf");
        appendExisting(out, "C:/Windows/Fonts/verdana.ttf");
        appendExisting(out, "C:/Windows/Fonts/tahoma.ttf");
        appendExisting(out, "C:/Windows/Fonts/segoeui.ttf");
#elif defined(__APPLE__)
        appendExisting(out, "/System/Library/Fonts/Helvetica.ttc");
        appendExisting(out, "/Library/Fonts/Arial.ttf");
#else
        if (bold) {
            appendExisting(out, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf");
            appendExisting(out, "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
            appendExisting(out, "/usr/share/fonts/noto/NotoSans-Bold.ttf");
        }
        appendExisting(out, "/usr/share/fonts/TTF/DejaVuSans.ttf");
        appendExisting(out, "/usr/share/fonts/noto/NotoSans-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        appendExisting(out, "/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/TTF/LiberationSans-Regular.ttf");
        appendExisting(out, "/usr/share/fonts/gsfonts/NimbusSans-Regular.otf");
        if (!bold) {
            appendExisting(out, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf");
        }
#endif
    }
};
