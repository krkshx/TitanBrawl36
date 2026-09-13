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

static void present(LoadingScreen &screen, pc::Window &window) {
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
    present(screen, window);
    bool textsOk = screen.loadTexts(assetsDir);
    present(screen, window);
    std::string scPath = FileSystem::join(assetsDir, FileSystem::join("sc", "loading.sc"));
    bool clipOk = screen.loadClip(scPath);
    screen.showLogo();
    screen.setProgress(0.0f);
    present(screen, window);
    std::int64_t logoUntil = Clock::nowMs() + 1500;
    while (window.poll() && Clock::nowMs() < logoUntil) {
        screen.draw(window.frame(), window.width(), window.height());
        window.present();
        Clock::sleepMs(16);
    }
    screen.showLoading();
    screen.setProgress(0.05f);
    present(screen, window);
    SupercellSWF ui;
    std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
    bool uiOk = ui.load(uiPath);
    screen.setProgress(0.3f);
    present(screen, window);
    std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
    bool uiTexOk = ui.loadTexture(uiTexPath);
    screen.setProgress(0.55f);
    present(screen, window);
    std::string csvPath = FileSystem::join(assetsDir, FileSystem::join("csv_logic", "characters.csv"));
    CsvTable table;
    bool csvOk = table.load(csvPath);
    screen.setProgress(0.75f);
    present(screen, window);
    ResetAccountMessage msg;
    msg.setPreset(1);
    msg.encode();
    ByteStream out;
    LogicDailyData daily;
    daily.a0_ = 10;
    daily.coins_ = 100;
    daily.encode(out);
    screen.setProgress(1.0f);
    present(screen, window);
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
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.id() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
