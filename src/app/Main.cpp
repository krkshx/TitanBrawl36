#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../titan/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../fmod/Music.cpp"
#include "../fmod/MusicLibrary.cpp"
#include "../fmod/LoadingSound.cpp"
#include "../fmod/Sfx.cpp"
#include "../fmod/SfxLibrary.cpp"
#include "../helpers/FileSystem.cpp"
#include "../platform/native/LoginError.cpp"
#include "../platform/native/NativeDialog.cpp"
#include "../helpers/Clock.cpp"
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
    if (!window.open(848, 480, "Laser")) {
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
    // === ПОЛНЫЙ БУТ (цикл): ЛОГО → ЗАГРУЗКА → КОННЕКТ → диалог.
    // "Попробовать снова" крутит всё с самого начала: сначала лого, потом лоадинг, как в ориге.
    SupercellSWF ui;
    CsvTable table;
    ResetAccountMessage msg;
    ByteStream out;
    LogicDailyData daily;
    bool uiOk = false;
    bool uiTexOk = false;
    bool csvOk = false;
    while (window.poll()) {
    // --- ЛОГО Supercell: крутим таймлайн sc_intro (100 кадров @60fps) + джингл one-shot. ---
    // Кадр — строго по wall-clock (софт-рендер ~30мс/кадр, фиксированный тик дрейфовал и лагал).
    screen.showLogo();
    screen.setProgress(0.0f);
    Sfx::play(LoadingSound::logoJingle(assetsDir));
    {
        int frames = screen.logoFrames();
        if (frames < 1) {
            frames = 100;
        }
        std::int64_t t0 = Clock::nowMs();
        for (;;) {
            if (!window.poll()) {
                break;
            }
            int fi = static_cast<int>((Clock::nowMs() - t0) * 60 / 1000);
            if (fi >= frames) {
                break;
            }
            screen.setLogoFrame(fi);
            present(screen, window);
        }
    }
    if (!window.poll()) {
        break;
    }
    // --- ЗАГРУЗКА: музыка загрузочного фона (sting, one-shot), бар — только от реальной работы. ---
    screen.showLoading();
    screen.setProgress(0.0f);
    present(screen, window);
    Music music;
    // Стинг загрузочного фона — one-shot БЕЗ цикла (в ориге не лупится).
    std::string loadMusic = LoadingSound::loadingMusic(assetsDir);
    if (!loadMusic.empty()) {
        music.start(loadMusic, false);
    }
    // Прямой бар по реальным майлстоунам (как было): 0 → 0.2 → 0.35 → 0.5, дальше фриз.
    ui = SupercellSWF();
    std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
    uiOk = ui.load(uiPath);
    screen.setProgress(0.2f);
    present(screen, window);
    std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
    uiTexOk = ui.loadTexture(uiTexPath, [&](int done, int total) {
        // Реальные готовые текстуры: 20% + доля отрезка 20→35. Никаких чужих значений.
        float f = total > 0 ? static_cast<float>(done) / static_cast<float>(total) : 1.0f;
        screen.setProgress(0.2f + f * 0.15f);
        present(screen, window);
    });
    screen.setProgress(0.35f);
    present(screen, window);
    std::string csvPath = FileSystem::join(assetsDir, FileSystem::join("csv_logic", "characters.csv"));
    table = CsvTable();
    csvOk = table.load(csvPath);
    msg = ResetAccountMessage();
    msg.setPreset(1);
    msg.encode();
    out = ByteStream();
    daily = LogicDailyData();
    daily.a0_ = 10;
    daily.coins_ = 100;
    daily.encode(out);
    screen.setProgress(0.5f);
    present(screen, window);
    // --- КОННЕКТ: бар ЗАМИРАЕТ, текст — TID_CONNECTING_TO_SERVER. ---
    // Кадр статичный: рендерим при ресайзе, иначе только blit.
    // (заглушка до net/login: вместо сети — сразу локальный фейл).
    screen.setConnecting();
    present(screen, window);
    {
        std::int64_t until = Clock::nowMs() + 2000;
        while (window.poll() && Clock::nowMs() < until) {
            if (window.takeResized()) {
                present(screen, window);
            } else {
                window.present();
            }
            Clock::sleepMs(50);
        }
    }
    if (!window.poll()) {
        break;
    }
    window.save(FileSystem::join(root, "frame.ppm"));
    // Сервер выключен — локальный коннект-фейл, поэтому CONNECTION_FAILED, а не LOGIN_FAILED.
    // Маппинг всех серверных кодов — в LoginError::askServerCode (задействуем когда заведём net/login).
    // false (Esc/крестик) — выходим, true (кнопка/Enter) — весь бут с самого лого.
    bool retry = LoginError::askConnectionFailed(screen.localization());
    if (!retry) {
        break;
    }
    } // while(window.poll()) — рестарт с лого
    window.save(FileSystem::join(root, "frame.ppm"));
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.id() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
