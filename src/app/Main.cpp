#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../titan/ui/UiButton.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../titan/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../net/core/Messaging.cpp"
#include "../fmod/Music.cpp"
#include "../fmod/MusicLibrary.cpp"
#include "../fmod/LoadingSound.cpp"
#include "../fmod/Sfx.cpp"
#include "../fmod/SfxLibrary.cpp"
#include "../helpers/FileSystem.cpp"
#include "../platform/native/NativeDialog.cpp"
#include "../helpers/Clock.cpp"
#include "../platform/window/Window.cpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Аккаунт между запусками: account.dat в корне (id + passToken).
static bool loadAccount(const std::string &root, std::int64_t &id, std::string &pass) {
    std::ifstream f(FileSystem::join(root, "account.dat"));
    if (!f) {
        return false;
    }
    std::string a, p;
    if (!std::getline(f, a) || !std::getline(f, p)) {
        return false;
    }
    try {
        id = std::stoll(a);
    } catch (...) {
        return false;
    }
    pass = p;
    return id != 0 && !pass.empty();
}

static void saveAccount(const std::string &root, std::int64_t id, const std::string &pass) {
    if (id == 0 || pass.empty()) {
        return;
    }
    std::ofstream f(FileSystem::join(root, "account.dat"), std::ios::trunc);
    f << id << "\n" << pass << "\n";
}

static std::string lowerHas(const std::string &name, const std::string &needle) {
    if (needle.size() > name.size()) {
        return "";
    }
    for (std::size_t i = 0; i + needle.size() <= name.size(); i++) {
        bool ok = true;
        for (std::size_t k = 0; k < needle.size(); k++) {
            char a = name[i + k];
            char b = needle[k];
            if (a >= 'A' && a <= 'Z') {
                a = static_cast<char>(a + 32);
            }
            if (b >= 'A' && b <= 'Z') {
                b = static_cast<char>(b + 32);
            }
            if (a != b) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return name;
        }
    }
    return "";
}

static void present(LoadingScreen &screen, UiButton *button, pc::Window &window) {
    screen.draw(window.frame(), window.width(), window.height());
    if (button) {
        button->draw(window.frame(), window.width(), window.height());
    }
    window.present();
}
// Клики по кнопке входа: статус-текст становится заданным. true — попали по кнопке.
static bool drainEntryClicks(pc::Window &window, UiButton *button, LoadingScreen &screen) {
    if (!button || !button->bound()) {
        return false;
    }
    bool hit = false;
    int cx = 0;
    int cy = 0;
    while (window.takeClick(cx, cy)) {
        if (button->hit(cx, cy, window.width(), window.height())) {
            screen.setStatusOverride("pmdrk loh...");
            hit = true;
        }
    }
    return hit;
}

// Меню после OwnHomeData: кнопки из ui.sc, кипалайв, памп входящих.
// true — окно закрыли (выход из игры), false — коннект потерян (ребут).
static bool homeMenu(LoadingScreen &screen, pc::Window &window, SupercellSWF &ui, ClientMessaging &net, std::int64_t accountId) {
    screen.setStatusOverride("MENU id=" + std::to_string(accountId));
    // Какие экспорты вообще есть под кнопки — в лог, чтобы подобрать иголки.
    int listed = 0;
    for (std::size_t i = 0; i < ui.exports.size() && listed < 30; i++) {
        const std::string &n = ui.exports[i].name;
        if (!lowerHas(n, "button").empty() || !lowerHas(n, "play").empty() ||
            !lowerHas(n, "shop").empty() || !lowerHas(n, "brawl").empty() ||
            !lowerHas(n, "menu").empty()) {
            std::cout << "uiexport: " << n << "\n";
            listed++;
        }
    }
    UiButton play, shop, brawlers;
    bool playOk = play.bind(&ui, "play");
    bool shopOk = shop.bind(&ui, "shop");
    bool brawlOk = brawlers.bind(&ui, "brawler");
    if (!brawlOk) {
        brawlOk = brawlers.bind(&ui, "button");
    }
    std::cout << "menu buttons play=" << (playOk ? play.assetName() : "-")
              << " shop=" << (shopOk ? shop.assetName() : "-")
              << " brawlers=" << (brawlOk ? brawlers.assetName() : "-") << "\n";
    // Подписи как в ориге (GameButton::setText): текст поверх скина кнопки.
    play.setLabel("PLAY");
    shop.setLabel("SHOP");
    brawlers.setLabel("BRAWLERS");
    std::int64_t lastAlive = Clock::nowMs();
    while (window.poll()) {
        int w = window.width();
        int h = window.height();
        // Ряд кнопок по центру низа: ориг так же центрирует пару через getX/setX
        // (сдвиг на -(x1+x2)/2) — считаем общую ширину ряда и стартуем от центра.
        UiButton *row[3] = {nullptr, nullptr, nullptr};
        int rowCount = 0;
        if (brawlOk) {
            row[rowCount++] = &brawlers;
        }
        if (playOk) {
            row[rowCount++] = &play;
        }
        if (shopOk) {
            row[rowCount++] = &shop;
        }
        const int bw = 220;
        const int bh = 64;
        const int gap = 40;
        int rowW = rowCount > 0 ? rowCount * bw + (rowCount - 1) * gap : 0;
        int rowX = (w - rowW) / 2;
        int rowY = h - 24 - bh;
        for (int i = 0; i < rowCount; i++) {
            row[i]->setSlot(rowX + i * (bw + gap), rowY, bw, bh);
        }
        screen.draw(window.frame(), w, h);
        for (int i = 0; i < rowCount; i++) {
            row[i]->draw(window.frame(), w, h);
            if (!row[i]->label().empty()) {
                UiButton::Rect slot = row[i]->slotRect(w, h);
                screen.drawLabel(window.frame(), w, h, row[i]->label(),
                    static_cast<float>(slot.x), static_cast<float>(slot.y),
                    static_cast<float>(slot.w), static_cast<float>(slot.h),
                    0xFFFFFFFFu, true, 0xFF000000u);
            }
        }
        window.present();
        int cx = 0;
        int cy = 0;
        while (window.takeClick(cx, cy)) {
            for (int i = 0; i < rowCount; i++) {
                if (!row[i]->hit(cx, cy, w, h)) {
                    continue;
                }
                const std::string &caption = row[i]->label().empty() ? row[i]->assetName() : row[i]->label();
                screen.setStatusOverride(caption + " pressed...");
                std::cout << "menu: " << caption << "\n";
            }
        }
        // Кипалайв + памп серверных пушей.
        if (Clock::nowMs() - lastAlive > 5000) {
            lastAlive = Clock::nowMs();
            KeepAliveMessage ka;
            if (!net.sendGame(ka)) {
                screen.setStatusOverride("Connection lost. Rebooting...");
                std::cout << "menu: keepalive send fail: " << net.lastError() << "\n";
                return false;
            }
        }
        PiranhaMessage *m = net.receiveNext(20);
        if (m) {
            std::cout << "menu: incoming type=" << m->getMessageType() << "\n";
            if (m->getMessageType() == 20103) {
                screen.setStatusOverride("Server kicked us. Rebooting...");
                delete m;
                return false;
            }
            delete m;
        } else if (!net.lastError().empty() && net.lastError() != "recv timeout") {
            screen.setStatusOverride("Connection lost. Rebooting...");
            std::cout << "menu: pump fail: " << net.lastError() << "\n";
            return false;
        }
        Clock::sleepMs(30);
    }
    return true;
}

int main(int argc, char **argv) {
    // Кнопка входа выключена по умолчанию, включается флагом --entry-button.
    std::string root = ".";
    bool entryButtonOn = false;
    bool rootSet = false;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--entry-button") {
            entryButtonOn = true;
        } else if (!rootSet && !a.empty() && a[0] != '-') {
            root = a;
            rootSet = true;
        }
    }
    pc::Window window;
    if (!window.open(848, 480, "Laser")) {
        std::cout << "window=0" << "\n";
        return 4;
    }
    LoadingScreen screen;
    UiButton entryButton;
    bool buttonOk = false;
    std::string assetsDir = FileSystem::join(root, "assets");
    present(screen, &entryButton, window);
    bool textsOk = screen.loadTexts(assetsDir);
    present(screen, &entryButton, window);
    std::string scPath = FileSystem::join(assetsDir, FileSystem::join("sc", "loading.sc"));
    bool clipOk = screen.loadClip(scPath);


    if (!clipOk) {
        // Без loading.sc экран остаётся чёрным на всех стадиях — сказать прямо, а не молчать.
        NativeDialog::show("Missing assets",
            "Could not load " + scPath + ".\nCopy the decompiled assets/ folder (sc/, csv_logic/, sfx/) next to the game and run again.",
            "OK");
    }
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
                drainEntryClicks(window, &entryButton, screen);
                int fi = static_cast<int>((Clock::nowMs() - t0) * 60 / 1000);
                if (fi >= frames) {
                    break;
                }
                screen.setLogoFrame(fi);
                present(screen, &entryButton, window);
            }
        }
        if (!window.poll()) {
            break;
        }
        screen.showLoading();
        screen.setProgress(0.0f);
        present(screen, &entryButton, window);
        Music music;
        std::string loadMusic = LoadingSound::loadingMusic(assetsDir);
        if (!loadMusic.empty()) {
            music.start(loadMusic, false);
        }
        ui = SupercellSWF();
        std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
        uiOk = ui.load(uiPath);
        screen.setProgress(0.2f);
        present(screen, &entryButton, window);
        std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
        uiTexOk = ui.loadTexture(uiTexPath, [&](int done, int total) {
            float f = total > 0 ? static_cast<float>(done) / static_cast<float>(total) : 1.0f;
            screen.setProgress(0.2f + f * 0.15f);
            present(screen, &entryButton, window);
        });
        // Кнопка входа: первый экспорт ui.sc с "button" в имени, текстуры уже на месте.
        // Без флага --entry-button не биндится: не рисуется и клики не ловит.
        if (entryButtonOn) {
            buttonOk = entryButton.bind(&ui, "button");
        }
        screen.setProgress(0.35f);
        present(screen, &entryButton, window);
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
        present(screen, &entryButton, window);
        screen.setConnecting();
        present(screen, &entryButton, window);
        // TSP-логин: hello -> pepper -> цепочка до OwnHomeData.
        // Нативных диалогов нет: ошибка — текстом на экране + автораetry.
        std::int64_t savedId = 0;
        std::string savedPass;
        bool haveSaved = loadAccount(root, savedId, savedPass);
        ClientMessaging net;
        bool loggedIn = false;
        std::size_t homeBytes = 0;
        std::int64_t accountId = 0;
        std::string netInfo = "offline";
        if (net.connect(5000)) {
            std::vector<std::uint8_t> token;
            if (net.sendClientHello() && net.receiveServerHello(token)) {
                LoginMessage login;
                if (haveSaved) {
                    login.setAccountId(savedId);
                    login.setPassToken(savedPass);
                } else {
                    login.setAccountId(0);
                    login.setPassToken("");
                }
                login.setVersion(36, 218);
                login.setRndKey(net.pathMtu());
                if (net.sendPepperLogin(login, token)) {
                    // Первый ответ — pepper-бокс (CreateAccountOk | LoginOk | LoginFailed),
                    // дальше стрим: ждём LoginOk и OwnHomeData.
                    PiranhaMessage *first = net.receivePepperResponse(!haveSaved);
                    if (first != nullptr) {
                        netInfo = std::string("first=") + std::to_string(first->getMessageType());
                        bool failed = false;
                        auto note = [&](PiranhaMessage *m) {
                            if (m->getMessageType() == 26007) {
                                auto *c = static_cast<CreateAccountOkMessage *>(m);
                                if (c->accountId() != 0) {
                                    accountId = c->accountId();
                                    saveAccount(root, accountId, c->passToken());
                                }
                            } else if (m->getMessageType() == 20104) {
                                auto *o = static_cast<LoginOkMessage *>(m);
                                if (o->accountId() != 0) {
                                    accountId = o->accountId();
                                    saveAccount(root, accountId, o->passToken());
                                }
                            } else if (m->getMessageType() == 24101) {
                                auto *home = static_cast<OwnHomeDataMessage *>(m);
                                homeBytes = home->raw().size();
                                if (net.streamOn() || accountId != 0) {
                                    loggedIn = true;
                                }
                            } else if (m->getMessageType() == 20103) {
                                auto *f = static_cast<LoginFailedMessage *>(m);
                                netInfo += std::string(" loginFailed=") + std::to_string(f->errorCode());
                                failed = true;
                            }
                        };
                        note(first);
                        delete first;
                        for (int i = 0; i < 10 && !loggedIn && !failed; i++) {
                            PiranhaMessage *m = net.receiveNext(4000);
                            if (m == nullptr) {
                                netInfo += std::string(" wait-fail: ") + net.lastError();
                                break;
                            }
                            netInfo += std::string(" +") + std::to_string(m->getMessageType());
                            note(m);
                            delete m;
                        }
                    } else {
                        netInfo = std::string("pepper-response-fail: ") + net.lastError();
                    }
                } else {
                    netInfo = std::string("pepper-login-fail: ") + net.lastError();
                }
            } else {
                netInfo = std::string("hello-fail: ") + net.lastError();
            }
        } else {
            netInfo = std::string("connect-fail: ") + net.lastError();
        }
        std::cout << "net=" << (loggedIn ? 1 : 0) << " " << netInfo
                  << " account=" << accountId << " homeBytes=" << homeBytes << "\n";
        if (!loggedIn) {
            // Без диалога: причина на экране, пауза с прокачкой окна, ребут с лого.
            screen.setStatusOverride("Connection failed: " + netInfo + ". Retrying...");
            present(screen, &entryButton, window);
            std::int64_t until = Clock::nowMs() + 3000;
            while (window.poll() && Clock::nowMs() < until) {
                if (window.takeResized()) {
                    present(screen, &entryButton, window);
                } else {
                    window.present();
                }
                Clock::sleepMs(100);
            }
            continue;
        }
        // OwnHomeData пройден: загрузка дальше как в ориге, затем меню.
        screen.setStatusOverride("Loading home...");
        screen.setProgress(0.85f);
        present(screen, &entryButton, window);
        Clock::sleepMs(400);
        screen.setStatusOverride("Entering menu...");
        screen.setProgress(1.0f);
        present(screen, &entryButton, window);
        window.save(FileSystem::join(root, "frame.ppm"));
        bool closed = homeMenu(screen, window, ui, net, accountId);
        window.save(FileSystem::join(root, "frame.ppm"));
        if (closed) {
            break;
        }
    }
    window.save(FileSystem::join(root, "frame.ppm"));
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.getMessageType() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " button=" << (buttonOk ? entryButton.assetName() : "-") << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
