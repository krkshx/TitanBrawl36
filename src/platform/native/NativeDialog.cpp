#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <X11/keysym.h>
#endif

class NativeDialog {
public:
    static void show(const std::string &title, const std::string &message, const std::string &button) {
        const char *skip = std::getenv("TITAN_NO_DIALOG");
        if (skip && skip[0] != '\0' && !(skip[0] == '0' && skip[1] == '\0')) {
            return;
        }
#if defined(_WIN32)
        showWindows(title, message, button);
#else
        showX11(title, message, button);
#endif
    }
private:
#if defined(_WIN32)
    static std::wstring utf8(const std::string &value) {
        if (value.empty()) {
            return std::wstring();
        }
        int n = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, nullptr, 0);
        if (n <= 0) {
            return std::wstring();
        }
        std::wstring out(static_cast<std::size_t>(n), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, &out[0], n);
        if (!out.empty() && out.back() == L'\0') {
            out.pop_back();
        }
        return out;
    }
    static void showWindows(const std::string &title, const std::string &message, const std::string &button) {
        (void)button;
        std::wstring t = utf8(title);
        std::wstring m = utf8(message);
        MessageBoxW(nullptr, m.c_str(), t.c_str(), MB_OK | MB_ICONERROR);
    }
#else
    struct Line {
        std::string text;
    };
    static void wrapText(Display *display, XftFont *font, const std::string &text, int maxW, std::vector<Line> &lines) {
        std::size_t pos = 0;
        while (pos <= text.size()) {
            std::size_t nl = text.find('\n', pos);
            std::string para = text.substr(pos, nl == std::string::npos ? std::string::npos : nl - pos);
            std::size_t word = 0;
            std::string cur;
            for (;;) {
                std::size_t sp = para.find(' ', word);
                std::string tok = para.substr(word, sp == std::string::npos ? std::string::npos : sp - word);
                std::string add = cur.empty() ? tok : cur + " " + tok;
                XGlyphInfo ext;
                XftTextExtentsUtf8(display, font, reinterpret_cast<const FcChar8 *>(add.c_str()), static_cast<int>(add.size()), &ext);
                if (static_cast<int>(ext.xOff) > maxW && !cur.empty()) {
                    Line line;
                    line.text = cur;
                    lines.push_back(line);
                    cur = tok;
                } else {
                    cur = add;
                }
                if (sp == std::string::npos) {
                    break;
                }
                word = sp + 1;
            }
            Line line;
            line.text = cur;
            lines.push_back(line);
            if (nl == std::string::npos) {
                break;
            }
            pos = nl + 1;
        }
    }
    static int lineHeight(XftFont *font) {
        return font->ascent + font->descent + 4;
    }
    static void showX11(const std::string &title, const std::string &message, const std::string &button) {
        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            return;
        }
        int screen = DefaultScreen(display);
        int scrW = DisplayWidth(display, screen);
        int scrH = DisplayHeight(display, screen);
        const int winW = 480;
        const int maxTextW = winW - 64;
        XftFont *titleFont = XftFontOpenName(display, screen, "sans-11:bold");
        XftFont *bodyFont = XftFontOpenName(display, screen, "sans-9");
        XftFont *buttonFont = XftFontOpenName(display, screen, "sans-9:bold");
        if (!bodyFont) {
            if (titleFont) {
                XftFontClose(display, titleFont);
            }
            XCloseDisplay(display);
            return;
        }
        if (!titleFont) {
            titleFont = bodyFont;
        }
        if (!buttonFont) {
            buttonFont = bodyFont;
        }
        std::vector<Line> lines;
        wrapText(display, bodyFont, message, maxTextW, lines);
        if (lines.empty()) {
            Line line;
            lines.push_back(line);
        }
        int titleH = lineHeight(titleFont) + 10;
        int textH = static_cast<int>(lines.size()) * lineHeight(bodyFont);
        int btnW = 200;
        int btnH = 34;
        XGlyphInfo btnExt;
        XftTextExtentsUtf8(display, buttonFont, reinterpret_cast<const FcChar8 *>(button.c_str()), static_cast<int>(button.size()), &btnExt);
        if (static_cast<int>(btnExt.xOff) + 48 > btnW) {
            btnW = static_cast<int>(btnExt.xOff) + 48;
        }
        int winH = titleH + textH + btnH + 64;
        int winX = (scrW - winW) / 2;
        int winY = (scrH - winH) / 2;
        if (winX < 0) {
            winX = 0;
        }
        if (winY < 0) {
            winY = 0;
        }
        Window win = XCreateSimpleWindow(display, RootWindow(display, screen), winX, winY, static_cast<unsigned>(winW), static_cast<unsigned>(winH), 1, BlackPixel(display, screen), WhitePixel(display, screen));
        XStoreName(display, win, title.c_str());
        Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(display, win, &wmDelete, 1);
        XSelectInput(display, win, ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask);
        XMapWindow(display, win);
        XftDraw *draw = XftDrawCreate(display, win, DefaultVisual(display, screen), DefaultColormap(display, screen));
        XftColor black;
        XftColor white;
        XftColor gray;
        XftColor darkGray;
        XftColorAllocName(display, DefaultVisual(display, screen), DefaultColormap(display, screen), "black", &black);
        XftColorAllocName(display, DefaultVisual(display, screen), DefaultColormap(display, screen), "white", &white);
        XftColorAllocName(display, DefaultVisual(display, screen), DefaultColormap(display, screen), "gray75", &gray);
        XftColorAllocName(display, DefaultVisual(display, screen), DefaultColormap(display, screen), "gray40", &darkGray);
        int btnX = (winW - btnW) / 2;
        int btnY = winH - btnH - 20;
        bool done = false;
        bool mapped = false;
        while (!done) {
            XEvent e;
            XNextEvent(display, &e);
            if (e.type == MapNotify) {
                mapped = true;
            }
            if (e.type == Expose && e.xexpose.count == 0) {
                XClearWindow(display, win);
                int ty = 12 + titleFont->ascent;
                XftDrawStringUtf8(draw, &black, titleFont, 24, ty, reinterpret_cast<const FcChar8 *>(title.c_str()), static_cast<int>(title.size()));
                int y = titleH + 8 + bodyFont->ascent;
                for (std::size_t i = 0; i < lines.size(); i++) {
                    XftDrawStringUtf8(draw, &black, bodyFont, 32, y, reinterpret_cast<const FcChar8 *>(lines[i].text.c_str()), static_cast<int>(lines[i].text.size()));
                    y += lineHeight(bodyFont);
                }
                XSetForeground(display, DefaultGC(display, screen), 0xD4D0C8u);
                XFillRectangle(display, win, DefaultGC(display, screen), btnX, btnY, static_cast<unsigned>(btnW), static_cast<unsigned>(btnH));
                XSetForeground(display, DefaultGC(display, screen), 0x000000u);
                XDrawRectangle(display, win, DefaultGC(display, screen), btnX, btnY, static_cast<unsigned>(btnW), static_cast<unsigned>(btnH));
                int labelW = static_cast<int>(btnExt.xOff);
                int bx = btnX + (btnW - labelW) / 2;
                int by = btnY + (btnH + buttonFont->ascent - buttonFont->descent) / 2;
                XftDrawStringUtf8(draw, &black, buttonFont, bx, by, reinterpret_cast<const FcChar8 *>(button.c_str()), static_cast<int>(button.size()));
                (void)mapped;
                (void)white;
                (void)gray;
                (void)darkGray;
            }
            if (e.type == ButtonPress) {
                int x = e.xbutton.x;
                int y = e.xbutton.y;
                if (x >= btnX && x < btnX + btnW && y >= btnY && y < btnY + btnH) {
                    done = true;
                }
            }
            if (e.type == KeyPress) {
                KeySym sym = XLookupKeysym(&e.xkey, 0);
                if (sym == XK_Return || sym == XK_KP_Enter || sym == XK_Escape || sym == XK_space) {
                    done = true;
                }
            }
            if (e.type == ClientMessage) {
                if (static_cast<Atom>(e.xclient.data.l[0]) == wmDelete) {
                    done = true;
                }
            }
            if (e.type == DestroyNotify) {
                done = true;
            }
        }
        XftDrawDestroy(draw);
        if (buttonFont != bodyFont) {
            XftFontClose(display, buttonFont);
        }
        if (titleFont != bodyFont) {
            XftFontClose(display, titleFont);
        }
        XftFontClose(display, bodyFont);
        XDestroyWindow(display, win);
        XCloseDisplay(display);
    }
#endif
};
