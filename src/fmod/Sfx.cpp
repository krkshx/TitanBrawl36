#pragma once
#include "Music.cpp"
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>

// Второй голос (one-shot SFX) — идёт поверх loop-музыки отдельным pulse-стримом, как в ориге
// (джингл лого overlapped с началом загрузочной музыки).
// Fire-and-forget: поток живёт пока звучит файл, своего ничего не микшируем.
class Sfx {
public:
    static void play(const std::string &path) {
        if (path.empty() || !exists(path)) {
            return;
        }
        std::thread([path]() {
            Music m;
            if (!m.start(path, false)) {
                return;
            }
            for (int i = 0; i < 100 && !m.playing(); i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            while (m.playing()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }).detach();
    }

private:
    static bool exists(const std::string &path) {
        FILE *h = std::fopen(path.c_str(), "rb");
        if (!h) {
            return false;
        }
        std::fclose(h);
        return true;
    }
};
