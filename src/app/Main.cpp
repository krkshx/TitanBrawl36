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
#include "../logic/home/LogicClientHome.cpp"
#include "../platform/native/NativeDialog.cpp"
#include "../helpers/Clock.cpp"
#include "../platform/window/Window.cpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

static void blendPixel(std::vector<std::uint32_t> &frame, int w, int h, int x, int y, std::uint32_t src) {
    if (x < 0 || y < 0 || x >= w || y >= h) {
        return;
    }
    unsigned sa = (src >> 24) & 0xFF;
    if (sa == 0) {
        return;
    }
    std::size_t k = static_cast<std::size_t>(y) * static_cast<std::size_t>(w) + static_cast<std::size_t>(x);
    if (sa == 255) {
        frame[k] = 0xFF000000u | (src & 0x00FFFFFFu);
        return;
    }
    std::uint32_t dst = frame[k];
    unsigned sr = (src >> 16) & 0xFF;
    unsigned sg = (src >> 8) & 0xFF;
    unsigned sb = src & 0xFF;
    unsigned dr = (dst >> 16) & 0xFF;
    unsigned dg = (dst >> 8) & 0xFF;
    unsigned db = dst & 0xFF;
    unsigned r = (sr * sa + dr * (255 - sa)) / 255;
    unsigned g = (sg * sa + dg * (255 - sa)) / 255;
    unsigned b = (sb * sa + db * (255 - sa)) / 255;
    frame[k] = 0xFF000000u | (r << 16) | (g << 8) | b;
}

static void fillRect(std::vector<std::uint32_t> &frame, int w, int h,
                     int x0, int y0, int rw, int rh, std::uint32_t color) {
    for (int y = y0; y < y0 + rh; y++) {
        for (int x = x0; x < x0 + rw; x++) {
            blendPixel(frame, w, h, x, y, color);
        }
    }
}


static void fadeOver(std::vector<std::uint32_t> &frame, int w, int h, int alpha) {
    if (alpha <= 0) {
        return;
    }
    if (alpha > 255) {
        alpha = 255;
    }
    fillRect(frame, w, h, 0, 0, w, h, static_cast<std::uint32_t>(alpha) << 24);
}


static std::string themeMusicPath(const std::string &assetsDir, int themeId) {
    if (themeId <= 0) {
        return std::string();
    }
    int inst = themeId % 1000000;
    CsvTable themes;
    if (!themes.load(FileSystem::join(assetsDir, FileSystem::join("csv_logic", "themes.csv")))) {
        return std::string();
    }
    std::size_t row = static_cast<std::size_t>(inst + 1);
    if (row >= themes.rows()) {
        return std::string();
    }
    std::string tm = themes.get(row, "ThemeMusic");
    if (tm.empty()) {
        return std::string();
    }
    return MusicLibrary::fileForMusic(assetsDir, tm);
}

static void playClick(const std::string &assetsDir, bool brawl) {
    std::string f = brawl ? "sfx/brawl_button_01.ogg" : "sfx/menu_click_08.ogg";
    Sfx::play(FileSystem::join(assetsDir, f));
}


static int resolveThemeId(const std::vector<char> &raw) {
    if (!raw.empty()) {
        ByteStream hs;
        hs.setByteArray(raw.data(), static_cast<std::int32_t>(raw.size()));
        LogicClientHome lh;
        lh.decode(hs);
        for (std::size_t i = 0; i < lh.conf.intValues.size(); i++) {
            if (lh.conf.intValues[i].key == 1 && lh.conf.intValues[i].value > 0) {
                return lh.conf.intValues[i].value;
            }
        }
    }
    if (raw.size() >= 16) {
        for (std::size_t i = 0; i + 16 <= raw.size(); i++) {
            auto be32 = [&](std::size_t o) -> std::int32_t {
                return (static_cast<std::int32_t>(static_cast<unsigned char>(raw[o])) << 24) |
                       (static_cast<std::int32_t>(static_cast<unsigned char>(raw[o + 1])) << 16) |
                       (static_cast<std::int32_t>(static_cast<unsigned char>(raw[o + 2])) << 8) |
                       static_cast<std::int32_t>(static_cast<unsigned char>(raw[o + 3]));
            };
            std::int32_t k1 = be32(i);
            std::int32_t v1 = be32(i + 4);
            std::int32_t k2 = be32(i + 8);
            std::int32_t v2 = be32(i + 12);
            if (k1 == 1 && v1 >= 41000000 && v1 <= 41999999 && k2 == 50 && v2 == 1) {
                return v1;
            }
        }
    }
    return -1;
}

// Фон меню по теме (HomeScreen::enter: cover-масштаб, центрирование).
struct ThemeBg {
    SupercellSWF bg;
    ClipRenderer renderer;
    int exportId = -1;
    bool ok = false;
    std::string file;
    std::string name;
    std::string themeName;
    std::string themeMusic;
    int animClipId = -1;
    int animFrames = 0;
    int animFps = 30;
    bool load(const std::string &assetsDir, int themeId) {
        ok = false;
        if (themeId <= 0) {
            return false;
        }
        int inst = themeId % 1000000;
        CsvTable themes;
        if (!themes.load(FileSystem::join(assetsDir, FileSystem::join("csv_logic", "themes.csv")))) {
            return false;
        }
        std::size_t row = static_cast<std::size_t>(inst + 1);
        if (row >= themes.rows()) {
            return false;
        }
        file = themes.get(row, "FileName");
        name = themes.get(row, "ExportName");
        themeName = themes.get(row, "Name");
        themeMusic = themes.get(row, "ThemeMusic");
        if (file.empty() || name.empty()) {
            return false;
        }
        std::string scPath = FileSystem::join(assetsDir, file);
        if (!bg.load(scPath)) {
            return false;
        }
        std::string texPath = scPath;
        std::size_t dot = texPath.rfind('.');
        if (dot != std::string::npos) {
            texPath = texPath.substr(0, dot) + "_tex.sc";
            if (!bg.loadTexture(texPath)) {
                return false;
            }
        }
        exportId = -1;
        for (std::size_t i = 0; i < bg.exports.size(); i++) {
            if (bg.exports[i].name == name) {
                exportId = bg.exports[i].id;
                break;
            }
        }
        if (exportId < 0) {
            return false;
        }
        renderer.bind(&bg);

        animClipId = -1;
        animFrames = 0;
        animFps = 30;
        for (std::size_t i = 0; i < bg.clips.size(); i++) {
            if (bg.clips[i].id != exportId && bg.clips[i].frames.size() > 1) {
                animClipId = bg.clips[i].id;
                animFrames = static_cast<int>(bg.clips[i].frames.size());
                animFps = bg.clips[i].fps > 0 ? bg.clips[i].fps : 30;
                break;
            }
        }
        ok = true;
        return true;
    }
    void draw(std::vector<std::uint32_t> &frame, int w, int h) {
        if (!ok) {
            return;
        }

        std::int64_t now = Clock::nowMs();
        if (!cacheValid_ || cacheW_ != w || cacheH_ != h || cacheFile_ != file ||
            now - cacheT0_ > 66) {
            cache_.resize(static_cast<std::size_t>(w) * static_cast<std::size_t>(h));
            if (animClipId >= 0 && animFrames > 1) {
                int fi = static_cast<int>((now * animFps / 1000) % animFrames);
                renderer.setClipFrame(animClipId, fi);
            }
            renderer.renderCover(cache_, w, h, exportId, "bg_colour");
            cacheW_ = w;
            cacheH_ = h;
            cacheFile_ = file;
            cacheT0_ = now;
            cacheValid_ = true;
        }
        if (cache_.size() == frame.size()) {
            frame = cache_;
        } else {
            renderer.renderCover(frame, w, h, exportId, "bg_colour");
        }
    }
    // Кэш растра фона (частицы 15 FPS).
    std::vector<std::uint32_t> cache_;
    int cacheW_ = 0;
    int cacheH_ = 0;
    std::string cacheFile_;
    std::int64_t cacheT0_ = 0;
    bool cacheValid_ = false;
};

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
            screen.clearStatusOverride();
            hit = true;
        }
    }
    return hit;
}

// UTF-8 для поля ника: счёт кодпоинтов и удаление последнего.
static std::size_t utf8Length(const std::string &s) {
    std::size_t n = 0;
    for (std::size_t i = 0; i < s.size();) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        std::size_t adv = 1;
        if ((c & 0x80) == 0) {
            adv = 1;
        } else if ((c & 0xE0) == 0xC0) {
            adv = 2;
        } else if ((c & 0xF0) == 0xE0) {
            adv = 3;
        } else if ((c & 0xF8) == 0xF0) {
            adv = 4;
        }
        i += adv;
        n++;
    }
    return n;
}

static void utf8Pop(std::string &s) {
    while (!s.empty()) {
        unsigned char c = static_cast<unsigned char>(s.back());
        s.pop_back();
        if ((c & 0xC0) != 0x80) {
            break;
        }
    }
}

static std::string trimSpaces(const std::string &s) {
    std::size_t a = 0;
    while (a < s.size() && (s[a] == ' ' || s[a] == '\t')) {
        a++;
    }
    std::size_t b = s.size();
    while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t')) {
        b--;
    }
    return s.substr(a, b - a);
}

static const TextFieldOriginal *nameFieldById(const SupercellSWF &ui, int fieldId) {
    for (std::size_t i = 0; i < ui.fields.size(); i++) {
        if (ui.fields[i].id == fieldId) {
            return &ui.fields[i];
        }
    }
    return nullptr;
}

static bool nameForbidden(const std::string &chunk) {

    for (std::size_t i = 0; i < chunk.size();) {
        unsigned char c = static_cast<unsigned char>(chunk[i]);
        if (c == '<' || c == '>' || c == '&') {
            return true;
        }
        unsigned cp = 0;
        std::size_t adv = 1;
        if ((c & 0x80) == 0) {
            cp = c;
        } else if ((c & 0xE0) == 0xC0 && i + 1 < chunk.size()) {
            cp = ((c & 0x1F) << 6) | (chunk[i + 1] & 0x3F);
            adv = 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < chunk.size()) {
            cp = ((c & 0x0F) << 12) | ((chunk[i + 1] & 0x3F) << 6) | (chunk[i + 2] & 0x3F);
            adv = 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < chunk.size()) {
            cp = ((c & 0x07) << 18) | ((chunk[i + 1] & 0x3F) << 12) | ((chunk[i + 2] & 0x3F) << 6) | (chunk[i + 3] & 0x3F);
            adv = 4;
        } else {
            return true;
        }
        if (cp >= 0x1F300 && cp <= 0x1FAFF) {
            return true;
        }
        if (cp >= 0x2600 && cp <= 0x27BF) {
            return true;
        }
        if (cp == 0x200D || cp == 0xFE0F) {
            return true;
        }
        i += adv;
    }
    return false;
}

static int nameScreen(LoadingScreen &screen, pc::Window &window, SupercellSWF &ui,
                      const std::string &assetsDir, const std::vector<char> &homeRaw,
                      ClientMessaging &net) {
    screen.clearStatusOverride();
    const int kTitleId = 11116;
    const int kBodyId = 11190;
    const int kInputId = 11192;
    const int kInputBgShape = 9678;
    const int kOkClip = 11559;
    const TextFieldOriginal *titleF = nameFieldById(ui, kTitleId);
    const TextFieldOriginal *bodyF = nameFieldById(ui, kBodyId);
    const TextFieldOriginal *inputF = nameFieldById(ui, kInputId);
    if (!inputF) {
        std::cout << "name: input field missing\n";
        return 2;
    }
    UiButton inputBg;
    bool inputBgOk = inputBg.bindShape(&ui, kInputBgShape);
    UiButton ok;
    bool okBound = ok.bindClip(&ui, kOkClip);
    std::string title = screen.text("TID_ENTER_NAME_TITLE", "Welcome to Brawl Stars!");
    std::string body = screen.text("TID_ENTER_NAME_BODY", "What is your name?");
    ok.setLabel(screen.text("TID_CONTINUE", "Continue"));
    ThemeBg bg;
    bool bgOk = bg.load(assetsDir, resolveThemeId(homeRaw));
    std::string name;
    const std::size_t kMaxName = 15;
    std::int64_t fadeT0 = Clock::nowMs();
    std::int64_t lastAlive = Clock::nowMs();
    bool sending = false;
    std::int64_t sentAt = 0;
    bool confirmed = false;
    std::int64_t fadeOutT0 = 0;
    auto trySend = [&]() {
        std::string t = trimSpaces(name);
        if (t.empty() || sending) {
            return;
        }
        sending = true;
        sentAt = Clock::nowMs();
        ChangeAvatarNameMessage req;
        req.setName(t);
        if (!net.sendGame(req)) {
            std::cout << "name: send fail: " << net.lastError() << "\n";
            sending = false;
            return;
        }
        playClick(assetsDir, false);
        std::cout << "name: sent '" << t << "'\n";
    };
    while (window.poll()) {
        int w = window.width();
        int h = window.height();
        if (bgOk) {
            bg.draw(window.frame(), w, h);
        } else {
            screen.draw(window.frame(), w, h);
        }

        fillRect(window.frame(), w, h, 0, 0, w, h, 0xAA000000u);
        int cx = w / 2;
        // Заголовок (стиль поля title_txt из либы).
        std::uint32_t titleCol = titleF ? static_cast<std::uint32_t>(titleF->color) : 0xFFFFFFFFu;
        screen.drawLabel(window.frame(), w, h, title,
            static_cast<float>(cx - 270), 86.0f, 540.0f, 64.0f,
            titleCol, true, 0xFF000000u);
        // Вопрос (стиль body_txt).
        std::uint32_t bodyCol = bodyF ? static_cast<std::uint32_t>(bodyF->color) : 0xFFFFFFFFu;
        screen.drawLabel(window.frame(), w, h, body,
            static_cast<float>(cx - 220), 158.0f, 440.0f, 40.0f,
            bodyCol, bodyF ? bodyF->outline : false, 0xFF000000u);
        // Поле ввода: фон-шейп из либы + живой текст стилем поля.
        const int ix = cx - 240;
        const int iw = 480;
        const int iy = 214;
        const int ih = 64;
        if (inputBgOk) {
            inputBg.setSlot(ix, iy, iw, ih);
            inputBg.draw(window.frame(), w, h);
        }
        if (!name.empty()) {
            screen.drawLabel(window.frame(), w, h, name,
                static_cast<float>(ix + 8), static_cast<float>(iy),
                static_cast<float>(iw - 16), static_cast<float>(ih),
                static_cast<std::uint32_t>(inputF->color), false, 0xFF000000u);
        }
        if (!sending && (Clock::nowMs() / 500) % 2 == 0) {
            int textW = name.empty() ? 0 : screen.measureLabel(name, static_cast<float>(ih));
            int ccx = ix + iw / 2 + textW / 2;
            if (ccx < ix + iw - 12) {
                fillRect(window.frame(), w, h, ccx, iy + 12, 2, ih - 24, 0xFF222222u);
            }
        }
        // OK (кнопка button_continue из либы; disabled — вуаль как фрейм f1).
        const int bw = 220;
        const int bh = 72;
        int okX = cx - bw / 2;
        int okY = 300;
        std::string trimmed = trimSpaces(name);
        bool canOk = !trimmed.empty() && !sending;
        if (okBound) {
            ok.setSlot(okX, okY, bw, bh);
            ok.draw(window.frame(), w, h);
            screen.drawLabel(window.frame(), w, h, sending ? "..." : ok.label(),
                static_cast<float>(okX), static_cast<float>(okY),
                static_cast<float>(bw), static_cast<float>(bh),
                0xFFFFFFFFu, true, 0xFF000000u);
            if (!canOk) {
                fillRect(window.frame(), w, h, okX, okY, bw, bh, 0x88000000u);
            }
        }
        // Fade-in.
        int fadeAge = static_cast<int>(Clock::nowMs() - fadeT0);
        if (fadeAge < 250) {
            fadeOver(window.frame(), w, h, 255 - fadeAge * 255 / 250);
        }
        if (confirmed) {
            int foAge = static_cast<int>(Clock::nowMs() - fadeOutT0);
            if (foAge >= 200) {
                return 1;
            }
            fadeOver(window.frame(), w, h, foAge * 255 / 200);
        }
        window.present();
        // Ввод (max 15, forbidden из либы).
        std::string chunk;
        while (window.takeTextChunk(chunk)) {
            if (sending || nameForbidden(chunk)) {
                continue;
            }
            std::string probe = name + chunk;
            if (utf8Length(probe) <= kMaxName) {
                name = probe;
            }
        }
        int spec = 0;
        while (window.takeSpecialKey(spec)) {
            if (sending) {
                continue;
            }
            if (spec == pc::Window::KEY_BACKSPACE) {
                utf8Pop(name);
            } else if (spec == pc::Window::KEY_ENTER) {
                if (!trimSpaces(name).empty()) {
                    trySend();
                }
            }
        }
        int mx = 0;
        int my = 0;
        while (window.takeClick(mx, my)) {
            if (okBound && canOk && ok.hit(mx, my, w, h)) {
                trySend();
            }
        }
        if (sending && !confirmed) {
            PiranhaMessage *m = net.receiveNext(20);
            if (m) {
                std::cout << "name: incoming type=" << m->getMessageType() << "\n";
                if (m->getMessageType() == 20103) {
                    delete m;
                    return 2;
                }
                if (m->getMessageType() == 24111) {
                    confirmed = true;
                    fadeOutT0 = Clock::nowMs();
                }
                delete m;
            }
            if (!confirmed && Clock::nowMs() - sentAt > 6000) {
                std::cout << "name: confirm timeout, proceeding\n";
                confirmed = true;
                fadeOutT0 = Clock::nowMs();
            }
        }
        if (Clock::nowMs() - lastAlive > 5000) {
            lastAlive = Clock::nowMs();
            KeepAliveMessage ka;
            if (!net.sendGame(ka)) {
                std::cout << "name: keepalive send fail: " << net.lastError() << "\n";
                return 2;
            }
            PiranhaMessage *pm = net.receiveNext(20);
            if (pm) {
                if (pm->getMessageType() == 20103) {
                    delete pm;
                    return 2;
                }
                if (sending && pm->getMessageType() == 24111 && !confirmed) {
                    confirmed = true;
                    fadeOutT0 = Clock::nowMs();
                }
                delete pm;
            }
        } else if (!sending) {
            PiranhaMessage *pm = net.receiveNext(20);
            if (pm) {
                if (pm->getMessageType() == 20103) {
                    delete pm;
                    return 2;
                }
                delete pm;
            } else if (!net.lastError().empty() && net.lastError() != "recv timeout") {
                std::cout << "name: pump fail: " << net.lastError() << "\n";
                return 2;
            }
        }
        Clock::sleepMs(30);
    }
    return 0;
}

struct MenuHit {
    std::string name;
    std::string label;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

struct Mainscreen {
    std::string name;
    int clipId = -1;
};

struct MenuUi {
    // ТОЛЬКО маинскрины либы. Никаких отдельных кнопок.
    struct Mainscreen {
        std::string name;
        int clipId = -1;
    };
    std::vector<Mainscreen> mainscreens;
    SupercellSWF *ui = nullptr;
    ClipRenderer renderer;

    bool load(SupercellSWF *u, LoadingScreen &screen, const std::string &assetsDir) {
        mainscreens.clear();
        ui = u;
        if (!ui) {
            return false;
        }
        addMainscreen("mainscreen_hud_top");
        addMainscreen("mainscreen_hud_top_left");
        addMainscreen("mainscreen_hud_top_right");
        addMainscreen("mainscreen_hud_left");
        addMainscreen("mainscreen_hud_right");
        addMainscreen("mainscreen_hud_bottom_left");
        addMainscreen("mainscreen_hud_bottom_right");
        addMainscreen("mainscreen_center");
        if (mainscreens.empty()) {
            return false;
        }
        renderer.bind(ui);
        renderer.setAssetDir(assetsDir);
        renderer.clearButtonTexts();
        renderer.clearFieldTexts();
        renderer.clearClipFrames();
        renderer.clearLive();
        // Ивент-кнопки правого слота по дефолту скрыты (в ориге их гасит
        // HomePage по активным событиям; офлайн — нет событий):
        // connection (переподключение), raid/warning, spectate, alliance_war,
        // pro_league/championship/ranked (слот текущего ивента показывает
        // event_container), important_notice. Пустые слоты команды 2/3
        // (player_2/3_area) тоже скрыты — иначе их короны/пины/скор рисуются
        // поверх панелей инвайтов. Остаются brawl_container (ИГРАТЬ),
        // event_container, rank_mode_label, player_1_area.
        // TODO: зажигать по парсу событий LogicClientHome из OwnHomeData.
        static const int kHiddenByDefault[] = {
            10265, 10277, 10278, 10279, 10281, 10283, 10285, 10294, 10319,
            10424, 10429,
        };
        for (std::size_t hi = 0; hi < sizeof(kHiddenByDefault) / sizeof(kHiddenByDefault[0]); hi++) {
            renderer.setHiddenClip(kHiddenByDefault[hi], true);
        }
        // Подписи кнопок (setTextByTID в ориге) — тексты в их же поля.
        renderer.setButtonText(10274, screen.text("TID_BATTLE", "PLAY"));
        renderer.setButtonText(10183, screen.text("TID_NAVI_4", "Brawlers"));
        renderer.setButtonText(10187, screen.text("TID_NAVI_1", "Shop"));
        renderer.setButtonText(10185, screen.text("TID_NAVI_2", "News"));
        renderer.setButtonText(10191, screen.text("TID_NAVI_5", "Club"));
        renderer.setButtonText(10193, screen.text("TID_NAVI_7", "Friends"));
        for (std::size_t i = 0; i < mainscreens.size(); i++) {
            std::cout << "mainscreen :: " << mainscreens[i].name << " clip=" << mainscreens[i].clipId << "\n";
        }
        return true;
    }
    void addMainscreen(const std::string &name) {
        for (std::size_t i = 0; i < ui->exports.size(); i++) {
            if (ui->exports[i].name == name) {
                Mainscreen m;
                m.name = name;
                m.clipId = ui->exports[i].id;
                mainscreens.push_back(m);
                return;
            }
        }
        std::cout << "mainscreen MISS " << name << "\n";
    }
    // Якорение HUD-контейнеров 1-в-1 как в либе
    // (MovieClipHelper::createScreenContainer, 0x569748):
    //ステージ: ширина фиксирована 1288, высота адаптивная (v16 = h/s),
    // масштаб width-driven s = w/1288 (v12), screen = design*s без сдвигов.
    // Контейнеры ставятся origin в точки: центр (v15/2,v16/2), верх-центр
    // (v15/2,0), низ-центр (v15/2,v16), левый-центр (0,v16/2) и т.д. —
    // через setPixelSnappedXY, БЕЗ докинга по границам контента.
    // Контент внутри уже лежит относительно origin (top_bar ±630 вокруг
    // центра, top_right уходит влево от правого края и т.п. — замерено).
    // Старый bounds-докинг сдвигал всё на (min/center контента) и тащил
    // на экран припаркованные за вьюпортом варианты — «всё съехало».
    Matrix2x3 anchorBase(const std::string &name, int w, int h) const {
        float s = w / 1288.0f;
        if (s <= 0) {
            s = 1;
        }
        float dh = h / s;
        float px = 644.0f;
        float py = dh * 0.5f;
        if (name == "mainscreen_hud_top") {
            px = 644.0f;
            py = 0.0f;
        } else if (name == "mainscreen_hud_top_left") {
            px = 0.0f;
            py = 0.0f;
        } else if (name == "mainscreen_hud_top_right") {
            px = 1288.0f;
            py = 0.0f;
        } else if (name == "mainscreen_hud_left") {
            px = 0.0f;
            py = dh * 0.5f;
        } else if (name == "mainscreen_hud_right") {
            px = 1288.0f;
            py = dh * 0.5f;
        } else if (name == "mainscreen_hud_bottom_left") {
            px = 0.0f;
            py = dh;
        } else if (name == "mainscreen_hud_bottom_right") {
            px = 1288.0f;
            py = dh;
        } else {
            px = 644.0f;
            py = dh * 0.5f;
        }
        Matrix2x3 base;
        base.setIdentity();
        base.a = s;
        base.d = s;
        base.x = px * s;
        base.y = py * s;
        return base;
    }
    void draw(std::vector<std::uint32_t> &frame, int w, int h) {
        // Кэш статики HUD (как ThemeBg::cache_): полный рерастр 8 клипов
        // каждый кадр на CPU — главная причина лагов. Статика кэшируется,
        // анимация (клипы с >1 кадром) тикает с тем же шагом 66мс, что фон.
        std::int64_t now = Clock::nowMs();
        if (!cacheValid_ || cacheW_ != w || cacheH_ != h || now - cacheT0_ > 66) {
            renderer.setAnimate(true, now);
            for (std::size_t i = 0; i < mainscreens.size(); i++) {
                renderer.renderWithBase(frame, w, h, mainscreens[i].clipId,
                                        anchorBase(mainscreens[i].name, w, h));
            }
            cache_ = frame;
            cacheW_ = w;
            cacheH_ = h;
            cacheT0_ = now;
            cacheValid_ = true;
            return;
        }
        if (cache_.size() == frame.size()) {
            frame = cache_;
        } else {
            renderer.setAnimate(true, now);
            for (std::size_t i = 0; i < mainscreens.size(); i++) {
                renderer.renderWithBase(frame, w, h, mainscreens[i].clipId,
                                        anchorBase(mainscreens[i].name, w, h));
            }
        }
    }
    const MenuHit *hitAt(int fx, int fy) const {
        (void)fx;
        (void)fy;
        return nullptr;
    }
    std::vector<std::uint32_t> cache_;
    int cacheW_ = 0;
    int cacheH_ = 0;
    std::int64_t cacheT0_ = 0;
    bool cacheValid_ = false;
};

// Меню после OwnHomeData: фон темы + кнопки-скины из ui.sc, кипалайв, памп входящих.
// true — окно закрыли (выход из игры), false — коннект потерян (ребут).
static void drawMenuFrame(LoadingScreen &screen, std::vector<std::uint32_t> &frame, int w, int h,
                          ThemeBg &bg, bool bgOk, MenuUi &menu) {
    if (bgOk) {
        bg.draw(frame, w, h);
    } else {
        screen.draw(frame, w, h);
    }
    menu.draw(frame, w, h);
}

static bool homeMenu(LoadingScreen &screen, pc::Window &window, SupercellSWF &ui,
                     const std::string &assetsDir, const std::vector<char> &homeRaw,
                     ClientMessaging &net, Music &themeMusic) {
    screen.clearStatusOverride();
    std::vector<char> curHome = homeRaw;
    int themeId = resolveThemeId(curHome);
    ThemeBg bg;
    bool bgOk = bg.load(assetsDir, themeId);
    std::cout << "menu theme=" << themeId << " bg=" << (bgOk ? (bg.file + ":" + bg.name) : "-")
              << " (fallback=loading)\n";
    MenuUi menu;
    if (!menu.load(&ui, screen, assetsDir)) {
        std::cout << "menu: mainscreen clips missing, fallback to loading screen\n";
        screen.draw(window.frame(), window.width(), window.height());
        window.present();
        return true;
    }
    std::cout << "menu mainscreens=" << menu.mainscreens.size() << "\n";
    // Плавное появление меню (fade-in), как переход экранов в ориге.
    std::int64_t fadeT0 = Clock::nowMs();
    std::int64_t lastAlive = Clock::nowMs();
    while (window.poll()) {
        int w = window.width();
        int h = window.height();
        drawMenuFrame(screen, window.frame(), w, h, bg, bgOk, menu);
        // Fade-in первые 250 мс.
        int fadeAge = static_cast<int>(Clock::nowMs() - fadeT0);
        if (fadeAge < 250) {
            fadeOver(window.frame(), w, h, 255 - fadeAge * 255 / 250);
        }
        window.present();
        int cx = 0;
        int cy = 0;
        while (window.takeClick(cx, cy)) {
            const MenuHit *hit = menu.hitAt(cx, cy);
            if (hit) {
                std::cout << "menu: " << hit->name << "\n";
                playClick(assetsDir, hit->name == "brawl_button");
            }
        }
        // Кипалайв + памп серверных пушей.
        if (Clock::nowMs() - lastAlive > 5000) {
            lastAlive = Clock::nowMs();
            KeepAliveMessage ka;
            if (!net.sendGame(ka)) {
                std::cout << "menu: keepalive send fail: " << net.lastError() << "\n";
                return false;
            }
        }
        PiranhaMessage *m = net.receiveNext(20);
        if (m) {
            std::cout << "menu: incoming type=" << m->getMessageType() << "\n";
            if (m->getMessageType() == 20103) {
                delete m;
                return false;
            }
            if (m->getMessageType() == 24101) {
                // Сервер обновил home (смена темы и т.п.): фон и музыка — в моменте.
                OwnHomeDataMessage *ohd = static_cast<OwnHomeDataMessage *>(m);
                if (!ohd->raw().empty()) {
                    curHome = ohd->raw();
                    int nt = resolveThemeId(curHome);
                    if (nt > 0 && nt != themeId) {
                        themeId = nt;
                        bgOk = bg.load(assetsDir, themeId);
                        std::cout << "menu theme changed: " << themeId
                                  << " bg=" << (bgOk ? (bg.file + ":" + bg.name) : "-") << "\n";
                        std::string mp = themeMusicPath(assetsDir, themeId);
                        if (!mp.empty()) {
                            themeMusic.stop();
                            themeMusic.start(mp, true);
                        }
                    }
                }
            }
            delete m;
        } else if (!net.lastError().empty() && net.lastError() != "recv timeout") {
            std::cout << "menu: pump fail: " << net.lastError() << "\n";
            return false;
        }
        Clock::sleepMs(30);
    }
    return true;
}

int main(int argc, char **argv) {
    // Кнопка входа выключена по умолчанию, включается флагом --entry-button.
    // --menu-test/--name-test: офлайн-рендер в frame.ppm без сети (по home.bin).
    std::string root = ".";
    bool entryButtonOn = false;
    bool menuTest = false;
    bool nameTest = false;
    bool loadingTest = false;
    bool rootSet = false;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--entry-button") {
            entryButtonOn = true;
        } else if (a == "--menu-test") {
            menuTest = true;
        } else if (a == "--name-test") {
            nameTest = true;
        } else if (a == "--loading-test") {
            loadingTest = true;
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
            buttonOk = entryButton.bind(&ui, "popover_button_green");
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
        if (loadingTest) {
            // Офлайн-проверка лоадинга: прогресс-бар 50% в frame.ppm, выход.
            screen.clearStatusOverride();
            int w = window.width();
            int h = window.height();
            screen.setProgress(0.5f);
            screen.draw(window.frame(), w, h);
            window.present();
            window.save(FileSystem::join(root, "frame.ppm"));
            return 0;
        }
        if (menuTest) {
            // Офлайн-проверка меню: тема из home.bin, рендер кадра, выход.
            std::vector<char> testHome;
            {
                std::ifstream hf(FileSystem::join(root, "home.bin"), std::ios::binary);
                if (hf) {
                    testHome.assign(std::istreambuf_iterator<char>(hf), std::istreambuf_iterator<char>());
                }
            }
            int themeId = resolveThemeId(testHome);
            ThemeBg tbg;
            bool tbgOk = tbg.load(assetsDir, themeId);
            MenuUi tmenu;
            bool tmenuOk = tmenu.load(&ui, screen, assetsDir);
            std::cout << "menutest theme=" << themeId << " bg=" << (tbgOk ? (tbg.file + ":" + tbg.name) : "-")
                      << " mainscreens=" << tmenu.mainscreens.size() << " menu=" << tmenuOk << "\n";
            screen.clearStatusOverride();
            int w = window.width();
            int h = window.height();
            drawMenuFrame(screen, window.frame(), w, h, tbg, tbgOk, tmenu);
            window.present();
            window.save(FileSystem::join(root, "frame.ppm"));
            return 0;
        }
        if (nameTest) {
            // Офлайн-проверка экрана ника: попап поверх фона темы, выход.
            std::vector<char> testHome;
            {
                std::ifstream hf(FileSystem::join(root, "home.bin"), std::ios::binary);
                if (hf) {
                    testHome.assign(std::istreambuf_iterator<char>(hf), std::istreambuf_iterator<char>());
                }
            }
            ThemeBg tbg;
            bool tbgOk = tbg.load(assetsDir, resolveThemeId(testHome));
            screen.clearStatusOverride();
            int w = window.width();
            int h = window.height();
            if (tbgOk) {
                tbg.draw(window.frame(), w, h);
            } else {
                screen.draw(window.frame(), w, h);
            }
            // Тот же путь, что в nameScreen: ассеты попапа из либы.
            UiButton demoBg;
            demoBg.bindShape(&ui, 9678);
            UiButton demoOk;
            demoOk.bindClip(&ui, 11559);
            demoOk.setLabel(screen.text("TID_CONTINUE", "Continue"));
            const TextFieldOriginal *demoTitleF = nameFieldById(ui, 11116);
            const TextFieldOriginal *demoBodyF = nameFieldById(ui, 11190);
            const TextFieldOriginal *demoInputF = nameFieldById(ui, 11192);
            std::cout << "nametest bg=" << tbgOk << " inputbg=1"
                      << " title=" << (demoTitleF ? 1 : 0) << " body=" << (demoBodyF ? 1 : 0)
                      << " input=" << (demoInputF ? 1 : 0) << " ok=1\n";
            fillRect(window.frame(), w, h, 0, 0, w, h, 0xAA000000u);
            int cx = w / 2;
            screen.drawLabel(window.frame(), w, h,
                screen.text("TID_ENTER_NAME_TITLE", "Welcome to Brawl Stars!"),
                static_cast<float>(cx - 270), 86.0f, 540.0f, 64.0f,
                demoTitleF ? static_cast<std::uint32_t>(demoTitleF->color) : 0xFFFFFFFFu,
                true, 0xFF000000u);
            screen.drawLabel(window.frame(), w, h,
                screen.text("TID_ENTER_NAME_BODY", "What is your name?"),
                static_cast<float>(cx - 220), 158.0f, 440.0f, 40.0f,
                demoBodyF ? static_cast<std::uint32_t>(demoBodyF->color) : 0xFFFFFFFFu,
                demoBodyF ? demoBodyF->outline : false, 0xFF000000u);
            demoBg.setSlot(cx - 240, 214, 480, 64);
            demoBg.draw(window.frame(), w, h);
            screen.drawLabel(window.frame(), w, h, "Brawler",
                static_cast<float>(cx - 232), 214.0f, 464.0f, 64.0f,
                demoInputF ? static_cast<std::uint32_t>(demoInputF->color) : 0xFF000000u,
                false, 0xFF000000u);
            demoOk.setSlot(cx - 110, 300, 220, 72);
            demoOk.draw(window.frame(), w, h);
            screen.drawLabel(window.frame(), w, h, demoOk.label(),
                static_cast<float>(cx - 110), 300.0f, 220.0f, 72.0f,
                0xFFFFFFFFu, true, 0xFF000000u);
            window.present();
            window.save(FileSystem::join(root, "frame.ppm"));
            return 0;
        }
        screen.setConnecting();
        present(screen, &entryButton, window);
        // TSP-логин: hello -> pepper -> цепочка до OwnHomeData.
        // Нативных диалогов нет: ошибка — текстом на экране + автораetry.
        std::int64_t savedId = 0;
        std::string savedPass;
        bool haveSaved = loadAccount(root, savedId, savedPass);
        ClientMessaging net;
        bool loggedIn = false;
        bool freshAccount = false;
        std::size_t homeBytes = 0;
        std::vector<char> homeRaw;
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
                                    // Новый аккаунт создан в этой сессии: после home
                                    // ориг показывает экран ввода ника.
                                    freshAccount = true;
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
                                homeRaw = home->raw();
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
        // OwnHomeData пройден: бар докатывает до 100% сам (прогресс клипа),
        // своих текстов поверх в ориге нет.
        screen.clearStatusOverride();
        screen.setProgress(0.85f);
        present(screen, &entryButton, window);
        Clock::sleepMs(400);
        window.save(FileSystem::join(root, "frame.ppm"));
        // Музыка лобби по теме (в ориге с входа на HomeScreen), вместо загрузочной.
        music.stop();
        Music themeMusic;
        {
            std::string tmp = themeMusicPath(assetsDir, resolveThemeId(homeRaw));
            if (!tmp.empty()) {
                std::cout << "lobby music: " << tmp << "\n";
                themeMusic.start(tmp, true);
            }
        }
        // Новый аккаунт без ника: ориг сначала просит ввести имя (10212),
        // и только после OK показывает главное меню.
        if (freshAccount) {
            screen.clearStatusOverride();
            screen.setProgress(1.0f);
            present(screen, &entryButton, window);
            int nameRes = nameScreen(screen, window, ui, assetsDir, homeRaw, net);
            window.save(FileSystem::join(root, "frame.ppm"));
            if (nameRes == 0) {
                break;
            }
            if (nameRes == 2) {
                continue;
            }
            freshAccount = false;
        }
        screen.clearStatusOverride();
        screen.setProgress(1.0f);
        present(screen, &entryButton, window);
        bool closed = homeMenu(screen, window, ui, assetsDir, homeRaw, net, themeMusic);
        window.save(FileSystem::join(root, "frame.ppm"));
        if (closed) {
            break;
        }
    }
    window.save(FileSystem::join(root, "frame.ppm"));
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.getMessageType() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " button=" << (buttonOk ? entryButton.assetName() : "-") << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
