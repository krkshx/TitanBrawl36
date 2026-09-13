#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../logic/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../platform/fs/FileSystem.cpp"
#include "../platform/time/Clock.cpp"
#include "../platform/window/Window.cpp"
#include <cstdint>
#include <iostream>
#include <string>

static void presentStage(LoadingScreen &screen, pc::Window &window, float progress) {
    screen.setProgress(progress);
    screen.draw(window.frame(), window.width(), window.height());
    window.present();
}

int main(int argc, char **argv) {
    std::string root = ".";
    if (argc > 1) {
        root = argv[1];
    }
    pc::Window window;
    if (!window.open(848, 480, "TitanBrawl36")) {
        std::cout << "window=0" << "\n";
        return 4;
    }
    LoadingScreen screen;
    std::string assetsDir = FileSystem::join(root, "assets");
    presentStage(screen, window, 0.02f);
    bool textsOk = screen.loadTexts(assetsDir);
    presentStage(screen, window, 0.12f);
    std::string scPath = FileSystem::join(assetsDir, FileSystem::join("sc", "loading.sc"));
    bool clipOk = screen.loadClip(scPath);
    presentStage(screen, window, 0.55f);
    std::string csvPath = FileSystem::join(assetsDir, FileSystem::join("csv_logic", "characters.csv"));
    CsvTable table;
    bool csvOk = table.load(csvPath);
    presentStage(screen, window, 0.8f);
    ResetAccountMessage msg;
    msg.setPreset(1);
    msg.encode();
    ByteStream out;
    LogicDailyData daily;
    daily.a0_ = 10;
    daily.coins_ = 100;
    daily.encode(out);
    presentStage(screen, window, 1.0f);
    window.save(FileSystem::join(root, "frame.ppm"));
    std::int64_t prev = Clock::nowMs();
    while (window.poll()) {
        std::int64_t now = Clock::nowMs();
        float dt = static_cast<float>(now - prev) / 1000.0f;
        prev = now;
        if (dt > 0.1f) {
            dt = 0.1f;
        }
        screen.update(dt);
        screen.draw(window.frame(), window.width(), window.height());
        window.present();
        Clock::sleepMs(16);
    }
    window.save(FileSystem::join(root, "frame.ppm"));
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.id() << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
