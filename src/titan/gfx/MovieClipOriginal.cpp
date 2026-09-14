#pragma once
#include "ScReader.cpp"
#include <string>
#include <vector>

class MovieClipOriginal {
public:
    int id = 0;
    int tag = 0;
    int fps = 30;
    struct Child {
        int id = 0;
        int blend = 0;
        std::string name;
    };
    struct Element {
        int child = 0;
        int matrix = 0;
        int color = 0;
    };
    struct Frame {
        std::string label;
        bool hasLabel = false;
        std::vector<Element> elements;
    };
    std::vector<Child> children;
    std::vector<Frame> frames;
    int matrixBankIndex = 0;
    int bankIndex = 0;
    // Scaling grid либы (сабтег 31): left/top/right/bottom в единицах клипа.
    bool hasGrid = false;
    float gridLeft = 0;
    float gridTop = 0;
    float gridRight = 0;
    float gridBottom = 0;
    void load(ScReader &s, int t) {
        tag = t;
        id = s.readU16();
        fps = s.readU8();
        int frameCount = s.readU16();
        frames.resize(static_cast<std::size_t>(frameCount));
        if (t == 49) {
            int propCount = s.readU8();
            for (int i = 0; i < propCount; i++) {
                int pt = s.readU8();
                if (pt == 0) {
                    s.readBool();
                }
            }
        }
        int elementCount = s.readI32();
        std::vector<int> flat;
        flat.reserve(static_cast<std::size_t>(elementCount) * 3);
        for (int i = 0; i < elementCount * 3; i++) {
            flat.push_back(s.readU16());
        }
        int childCount = s.readU16();
        std::vector<int> childIds;
        for (int i = 0; i < childCount; i++) {
            childIds.push_back(s.readU16());
        }
        std::vector<int> blends;
        if (t == 12 || t >= 35) {
            for (int i = 0; i < childCount; i++) {
                blends.push_back(s.readU8());
            }
        } else {
            for (int i = 0; i < childCount; i++) {
                blends.push_back(0);
            }
        }
        children.resize(static_cast<std::size_t>(childCount));
        for (int i = 0; i < childCount; i++) {
            children[static_cast<std::size_t>(i)].id = childIds[static_cast<std::size_t>(i)];
            children[static_cast<std::size_t>(i)].blend = blends[static_cast<std::size_t>(i)];
            children[static_cast<std::size_t>(i)].name = s.readAscii();
        }
        int used = 0;
        int frameIndex = 0;
        while (true) {
            int ft = s.readU8();
            int fl = s.readI32();
            std::size_t end = s.position() + static_cast<std::size_t>(fl);
            if (ft == 0) {
                break;
            }
            if ((ft == 5 || ft == 11) && frameIndex < frameCount) {
                int ec = s.readU16();
                Frame &fr = frames[static_cast<std::size_t>(frameIndex)];
                fr.hasLabel = s.readAsciiNull(fr.label);
                if (ft == 11) {
                    for (int i = 0; i < ec; i++) {
                        Element e;
                        e.child = flat[static_cast<std::size_t>(used * 3)];
                        e.matrix = flat[static_cast<std::size_t>(used * 3 + 1)];
                        e.color = flat[static_cast<std::size_t>(used * 3 + 2)];
                        used++;
                        fr.elements.push_back(e);
                    }
                } else {
                    for (int i = 0; i < ec; i++) {
                        Element e;
                        e.child = s.readU16();
                        e.matrix = s.readU16();
                        e.color = s.readU16();
                        fr.elements.push_back(e);
                    }
                }
                frameIndex++;
            } else if (ft == 41) {
                // Индекс банка матриц клипа (ScMatrixBank): его элементы
                // смотрят в matrixBanks[bankIndex], а не в общий массив.
                if (s.position() < end) {
                    bankIndex = s.readU8();
                }
            } else if (ft == 31) {
                // Scaling grid либы (MovieClip::load, TAG_SCALING_GRID):
                // left/top/width/height твипами.
                if (s.position() + 16 <= end) {
                    gridLeft = s.readTwip();
                    gridTop = s.readTwip();
                    float gw = s.readTwip();
                    float gh = s.readTwip();
                    gridRight = gridLeft + gw;
                    gridBottom = gridTop + gh;
                    hasGrid = true;
                }
            }
            while (s.position() < end) {
                s.readU8();
            }
        }
    }
};
