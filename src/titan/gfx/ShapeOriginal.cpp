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
    }
};
