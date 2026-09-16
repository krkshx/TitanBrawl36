#pragma once
#include "ScReader.cpp"
#include <cstdint>
#include <string>
#include <vector>

class ShapeOriginal {
public:
    int id = 0;
    int tag = 0;
    struct Command {
        int texture = 0;
        std::vector<float> x;
        std::vector<float> y;
        std::vector<int> u;
        std::vector<int> v;
    };
    std::vector<Command> commands;
    // NineSlice либы живёт на клипе (scaling grid, сабтег 31),
    // а не на шейпе. Своего детекта по вершинам нет.
    bool nineSlice = false;
    std::vector<float> gridX;
    std::vector<float> gridY;
    void load(ScReader &s, int t) {
        tag = t;
        id = s.readU16();
        int count = s.readU16();
        if (t == 18) {
            s.readU16();
        }
        for (int i = 0; i < count; i++) {
            Command c;
            int ct = s.readU8();
            int cl = s.readI32();
            std::size_t end = s.position() + static_cast<std::size_t>(cl);
            if (ct == 4 || ct == 17 || ct == 22) {
                c.texture = s.readU8();
                int vc = 4;
                if (ct != 4) {
                    vc = s.readU8();
                }
                for (int k = 0; k < vc; k++) {
                    float px = s.readTwip();
                    float py = s.readTwip();
                    c.x.push_back(px);
                    c.y.push_back(py);
                    c.u.push_back(0);
                    c.v.push_back(0);
                }
                for (int k = 0; k < vc; k++) {
                    c.u[static_cast<std::size_t>(k)] = s.readU16();
                    c.v[static_cast<std::size_t>(k)] = s.readU16();
                }
                commands.push_back(c);
            }
            while (s.position() < end) {
                s.readU8();
            }
        }
        if (t == 18) {
            detectGrid();
        }
    }
    int gridIndexX(float v) const {
        for (std::size_t i = 0; i < gridX.size(); i++) {
            if (gridX[i] == v) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    int gridIndexY(float v) const {
        for (std::size_t i = 0; i < gridY.size(); i++) {
            if (gridY[i] == v) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
private:
    static void insertDistinct(std::vector<float> &out, float v) {
        for (std::size_t i = 0; i < out.size(); i++) {
            if (out[i] == v) {
                return;
            }
        }
        out.push_back(v);
    }
    static void sortAsc(std::vector<float> &out) {
        for (std::size_t i = 0; i < out.size(); i++) {
            for (std::size_t k = i + 1; k < out.size(); k++) {
                if (out[k] < out[i]) {
                    float t = out[i];
                    out[i] = out[k];
                    out[k] = t;
                }
            }
        }
    }
    void detectGrid() {
        // Сетка шейпа нужна ТОЛЬКО UiButton (9-slice растяжка скина кнопки
        // в слот — как GameButton в либе). ClipRenderer её больше не
        // использует: scaling grid либы (сабтег 31) живёт на MovieClip,
        // а поголовное применение shape-грида к HUD давало «волны».
        gridX.clear();
        gridY.clear();
        nineSlice = false;
        if (commands.size() < 2) {
            return;
        }
        for (std::size_t i = 0; i < commands.size(); i++) {
            const Command &c = commands[i];
            if (c.x.size() != 4) {
                return;
            }
            for (std::size_t k = 0; k < 4; k++) {
                insertDistinct(gridX, c.x[k]);
                insertDistinct(gridY, c.y[k]);
            }
        }
        if (gridX.size() < 2 || gridY.size() < 2) {
            gridX.clear();
            gridY.clear();
            return;
        }
        sortAsc(gridX);
        sortAsc(gridY);
        nineSlice = true;
    }
};
