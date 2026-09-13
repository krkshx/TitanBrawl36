#pragma once
#include "ScReader.cpp"
#include <cstdint>
#include <string>

class TextFieldOriginal {
public:
    int id = 0;
    int tag = 0;
    std::string font;
    std::int32_t color = 0;
    bool bold = false;
    bool italic = false;
    bool multiline = false;
    int align = 0;
    int size = 12;
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
    bool outline = false;
    std::string text;
    bool deviceFont = false;
    bool unkBoolean = false;
    std::int32_t outlineColor = 0;
    int unk32 = 0;
    int bendAngle = 0;
    bool autoAdjust = false;
    std::string anotherText;
    bool hasText = false;
    bool hasAnotherText = false;
    void load(ScReader &s, int t) {
        tag = t;
        id = s.readU16();
        font = s.readAscii();
        color = s.readI32();
        bold = s.readBool();
        italic = s.readBool();
        multiline = s.readBool();
        s.readBool();
        align = s.readU8();
        size = s.readU8();
        left = s.readI16();
        top = s.readI16();
        right = s.readI16();
        bottom = s.readI16();
        outline = s.readBool();
        hasText = s.readAsciiNull(text);
        if (t == 7) {
            return;
        }
        deviceFont = s.readBool();
        if (t == 15) {
            return;
        }
        if (t == 20) {
            unkBoolean = true;
            return;
        }
        outlineColor = s.readI32();
        if (t == 21 || t == 25) {
            return;
        }
        unk32 = s.readU16();
        s.readU16();
        unkBoolean = true;
        if (t == 33) {
            return;
        }
        bendAngle = s.readI16();
        if (t == 43) {
            return;
        }
        autoAdjust = s.readBool();
        if (t == 44) {
            return;
        }
        hasAnotherText = s.readAsciiNull(anotherText);
    }
};
