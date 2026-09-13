#pragma once
#include "ScReader.cpp"

class Matrix2x3 {
public:
    float a = 1;
    float b = 0;
    float c = 0;
    float d = 1;
    float x = 0;
    float y = 0;
    void load(ScReader &s, bool precise) {
        float div = precise ? 65535.0f : 1024.0f;
        a = static_cast<float>(s.readI32()) / div;
        b = static_cast<float>(s.readI32()) / div;
        c = static_cast<float>(s.readI32()) / div;
        d = static_cast<float>(s.readI32()) / div;
        x = s.readTwip();
        y = s.readTwip();
    }
    void setIdentity() {
        a = 1;
        b = 0;
        c = 0;
        d = 1;
        x = 0;
        y = 0;
    }
    float applyX(float px, float py) const {
        return px * a + py * c + x;
    }
    float applyY(float px, float py) const {
        return py * d + px * b + y;
    }
    void multiply(const Matrix2x3 &m) {
        float na = a * m.a + b * m.c;
        float nb = a * m.b + b * m.d;
        float nc = d * m.c + c * m.a;
        float nd = d * m.d + c * m.b;
        float nx = m.applyX(x, y);
        float ny = m.applyY(x, y);
        a = na;
        b = nb;
        c = nc;
        d = nd;
        x = nx;
        y = ny;
    }
};
