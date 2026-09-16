#pragma once
#include "ColorTransform.cpp"
#include "Lzma.cpp"
#include "Matrix2x3.cpp"
#include "MovieClipOriginal.cpp"
#include "ScReader.cpp"
#include "ShapeOriginal.cpp"
#include "SWFTexture.cpp"
#include "TextFieldOriginal.cpp"
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>
#include <thread>
#include <vector>

class SupercellSWF {
public:
    struct Export {
        int id = 0;
        std::string name;
    };
    bool load(const std::string &path) {
        filename = path;
        std::vector<std::uint8_t> file = readFile(path);
        if (file.size() < 26) {
            return false;
        }
        if (file[0] != 0x53 || file[1] != 0x43) {
            return false;
        }
        std::uint32_t version = (static_cast<std::uint32_t>(file[2]) << 24) | (static_cast<std::uint32_t>(file[3]) << 16) | (static_cast<std::uint32_t>(file[4]) << 8) | file[5];
        if (version != 1) {
            return false;
        }
        std::uint32_t hashLen = (static_cast<std::uint32_t>(file[6]) << 24) | (static_cast<std::uint32_t>(file[7]) << 16) | (static_cast<std::uint32_t>(file[8]) << 8) | file[9];
        if (hashLen != 16) {
            return false;
        }
        const std::uint8_t *pay = file.data() + 26;
        int props = pay[0];
        int lc = props % 9;
        int lp = (props / 9) % 5;
        int pb = props / 45;
        std::uint32_t dict = static_cast<std::uint32_t>(pay[1]) | (static_cast<std::uint32_t>(pay[2]) << 8) | (static_cast<std::uint32_t>(pay[3]) << 16) | (static_cast<std::uint32_t>(pay[4]) << 24);
        std::uint32_t outSize = static_cast<std::uint32_t>(pay[5]) | (static_cast<std::uint32_t>(pay[6]) << 8) | (static_cast<std::uint32_t>(pay[7]) << 16) | (static_cast<std::uint32_t>(pay[8]) << 24);
        std::vector<std::uint8_t> raw;
        if (!Lzma::decompress(pay + 9, file.size() - 35, raw, outSize, lc, lp, pb, dict)) {
            return false;
        }
        ScReader s(raw);
        int shapeCount = s.readU16();
        int clipCount = s.readU16();
        int textureCount = s.readU16();
        int fieldCount = s.readU16();
        int matrixCount = s.readU16();
        int colorCount = s.readU16();
        s.skip(5);
        int exportCount = s.readU16();
        std::vector<int> exportIds;
        for (int i = 0; i < exportCount; i++) {
            exportIds.push_back(s.readU16());
        }
        exports.clear();
        for (int i = 0; i < exportCount; i++) {
            Export e;
            e.id = exportIds[static_cast<std::size_t>(i)];
            e.name = s.readAscii();
            exports.push_back(e);
        }
        shapes.clear();
        shapes.resize(static_cast<std::size_t>(shapeCount));
        clips.clear();
        clips.resize(static_cast<std::size_t>(clipCount));
        textures.clear();
        textures.resize(static_cast<std::size_t>(textureCount));
        fields.clear();
        fields.resize(static_cast<std::size_t>(fieldCount));
        matrixBanks.clear();
        colorBanks.clear();
        expectedClips = clipCount;
        expectedShapes = shapeCount;
        expectedFields = fieldCount;
        expectedTextures = textureCount;
        return loadTags(s, false);
    }
    bool loadTexture(const std::string &path) {
        return loadTexture(path, nullptr);
    }
    // progress(done, total) дёргается из вызывающего потока после каждой готовой текстуры.
    bool loadTexture(const std::string &path, const std::function<void(int, int)> &progress) {
        std::vector<std::uint8_t> file = readFile(path);
        if (file.size() < 26) {
            return false;
        }
        if (file[0] != 0x53 || file[1] != 0x43) {
            return false;
        }
        const std::uint8_t *pay = file.data() + 26;
        int props = pay[0];
        int lc = props % 9;
        int lp = (props / 9) % 5;
        int pb = props / 45;
        std::uint32_t dict = static_cast<std::uint32_t>(pay[1]) | (static_cast<std::uint32_t>(pay[2]) << 8) | (static_cast<std::uint32_t>(pay[3]) << 16) | (static_cast<std::uint32_t>(pay[4]) << 24);
        std::uint32_t outSize = static_cast<std::uint32_t>(pay[5]) | (static_cast<std::uint32_t>(pay[6]) << 8) | (static_cast<std::uint32_t>(pay[7]) << 16) | (static_cast<std::uint32_t>(pay[8]) << 24);
        std::vector<std::uint8_t> raw;
        if (!Lzma::decompress(pay + 9, file.size() - 35, raw, outSize, lc, lp, pb, dict)) {
            return false;
        }
        ScReader s(raw);
        if (!loadTextureHeads(s)) {
            return false;
        }
        return decodeTextureRaws(progress);
    }
    // Фаза 1: быстрый проход по тегам, сырые байты текстур — в сторону (без тяжёлого декода).
    bool loadTextureHeads(ScReader &s) {
        texRaws_.clear();
        texRaws_.reserve(textures.size());
        std::size_t loadedTextures = 0;
        while (true) {
            int tag = s.readU8();
            std::int32_t length = s.readI32();
            if (length < 0) {
                return false;
            }
            std::size_t end = s.position() + static_cast<std::size_t>(length);
            if (tag == 0) {
                return loadedTextures == textures.size() && texRaws_.size() == textures.size();
            }
            if (isTextureTag(tag)) {
                if (loadedTextures >= textures.size()) {
                    return false;
                }
                std::vector<std::uint8_t> rawBlob;
                textures[loadedTextures].loadHead(s, tag, rawBlob);
                texRaws_.push_back(std::move(rawBlob));
                loadedTextures++;
            }
            while (s.position() < end) {
                s.readU8();
            }
        }
    }
    // Фаза 2: тяжёлый декод пикселей впараллель (по потоку на текстуру), прогресс — по готовности.
    bool decodeTextureRaws(const std::function<void(int, int)> &progress) {
        int total = static_cast<int>(textures.size());
        if (total < 1 || static_cast<int>(texRaws_.size()) != total) {
            return false;
        }
        if (progress) {
            progress(0, total);
        }
        std::vector<std::thread> workers;
        workers.reserve(static_cast<std::size_t>(total));
        for (int i = 0; i < total; i++) {
            workers.emplace_back([this, i]() {
                textures[static_cast<std::size_t>(i)].decodeRaw(texRaws_[static_cast<std::size_t>(i)]);
            });
        }
        for (int i = 0; i < total; i++) {
            workers[static_cast<std::size_t>(i)].join();
            if (progress) {
                progress(i + 1, total);
            }
        }
        texRaws_.clear();
        texRaws_.shrink_to_fit();
        return true;
    }
    int createMovieClip(const std::string &name) const {
        for (std::size_t i = 0; i < exports.size(); i++) {
            if (exports[i].name == name) {
                return getOriginalMovieClip(exports[i].id);
            }
        }
        return -1;
    }
    int getOriginalMovieClip(int id) const {
        for (std::size_t i = 0; i < clips.size(); i++) {
            if (clips[i].id == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    int findShape(int id) const {
        for (std::size_t i = 0; i < shapes.size(); i++) {
            if (shapes[i].id == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    int findTextField(int id) const {
        for (std::size_t i = 0; i < fields.size(); i++) {
            if (fields[i].id == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    std::vector<Export> exports;
    std::vector<SWFTexture> textures;
    std::vector<ShapeOriginal> shapes;
    std::vector<MovieClipOriginal> clips;
    std::vector<TextFieldOriginal> fields;
    // Банки матриц/трансформов как в либе (ScMatrixBank): банк 0 создаётся
    // из заголовка, каждый тег 42 открывает новый банк со своими счётчиками.
    // Индексы элементов клипов смотрят в банк своего клипа (bankIndex, сабтег 41).
    std::vector<std::vector<Matrix2x3>> matrixBanks;
    std::vector<std::vector<ColorTransform>> colorBanks;
    const Matrix2x3 *matrixAt(int bank, int idx) const {
        static Matrix2x3 ident;
        if (bank < 0 || bank >= static_cast<int>(matrixBanks.size()) || idx < 0 ||
            idx >= static_cast<int>(matrixBanks[static_cast<std::size_t>(bank)].size())) {
            return &ident;
        }
        return &matrixBanks[static_cast<std::size_t>(bank)][static_cast<std::size_t>(idx)];
    }
    const ColorTransform *colorAt(int bank, int idx) const {
        if (bank < 0 || bank >= static_cast<int>(colorBanks.size()) || idx < 0 ||
            idx >= static_cast<int>(colorBanks[static_cast<std::size_t>(bank)].size())) {
            return nullptr;
        }
        return &colorBanks[static_cast<std::size_t>(bank)][static_cast<std::size_t>(idx)];
    }
    bool useExternalTexture = false;
private:
    static std::vector<std::uint8_t> readFile(const std::string &path) {
        std::vector<std::uint8_t> r;
        FILE *f = fopen(path.c_str(), "rb");
        if (!f) {
            return r;
        }
        fseek(f, 0, SEEK_END);
        long n = ftell(f);
        fseek(f, 0, SEEK_SET);
        r.resize(n > 0 ? static_cast<std::size_t>(n) : 0);
        if (n > 0) {
            std::size_t got = fread(r.data(), 1, static_cast<std::size_t>(n), f);
            r.resize(got);
        }
        fclose(f);
        return r;
    }
    static bool isTextureTag(int t) {
        return t == 1 || t == 16 || t == 19 || t == 24 || t == 27 || t == 28 || t == 29 || t == 34;
    }
    static bool isShapeTag(int t) {
        return t == 2 || t == 18;
    }
    static bool isClipTag(int t) {
        // 49 = TAG_MOVIE_CLIP_6 (custom properties, SupercellFlash):
        // MovieClipOriginal::load его понимает, без него такие клипы
        // молча пропадали бы из clips.
        return t == 3 || t == 10 || t == 12 || t == 14 || t == 35 || t == 49;
    }
    static bool isFieldTag(int t) {
        return t == 7 || t == 15 || t == 20 || t == 21 || t == 25 || t == 33 || t == 43 || t == 44 || t == 46;
    }
    bool loadTags(ScReader &s, bool texFile) {
        std::size_t loadedShapes = 0;
        std::size_t loadedClips = 0;
        std::size_t loadedTextures = 0;
        std::size_t loadedFields = 0;
        if (!texFile) {
            matrixBanks.clear();
            colorBanks.clear();
            matrixBanks.emplace_back();
            colorBanks.emplace_back();
        }
        while (true) {
            int tag = s.readU8();
            std::int32_t length = s.readI32();
            if (length < 0) {
                return false;
            }
            std::size_t end = s.position() + static_cast<std::size_t>(length);
            if (tag == 0) {
                if (texFile) {
                    return loadedTextures == textures.size();
                }
                return loadedClips == clips.size() && loadedShapes == shapes.size() && loadedFields == fields.size();
            }
            if (isTextureTag(tag)) {
                if (loadedTextures >= textures.size()) {
                    return false;
                }
                textures[loadedTextures].load(s, tag, !useExternalTexture || texFile);
                loadedTextures++;
            } else if (isShapeTag(tag)) {
                if (loadedShapes >= shapes.size()) {
                    return false;
                }
                shapes[loadedShapes].load(s, tag);
                loadedShapes++;
            } else if (isClipTag(tag)) {
                if (loadedClips >= clips.size()) {
                    return false;
                }
                clips[loadedClips].load(s, tag);
                loadedClips++;
            } else if (isFieldTag(tag)) {
                if (loadedFields >= fields.size()) {
                    return false;
                }
                fields[loadedFields].load(s, tag);
                loadedFields++;
            } else if (tag == 8 || tag == 36) {
                Matrix2x3 m;
                m.load(s, tag == 36);
                if (matrixBanks.empty()) {
                    matrixBanks.emplace_back();
                }
                matrixBanks.back().push_back(m);
            } else if (tag == 9) {
                ColorTransform c;
                c.read(s);
                if (colorBanks.empty()) {
                    colorBanks.emplace_back();
                }
                colorBanks.back().push_back(c);
            } else if (tag == 26) {
                useExternalTexture = true;
            } else if (tag == 42) {
                // Новый банк матриц (ScMatrixBank): u16 matrixCount + u16 colorCount,
                // дальше теги 8/36 и 9 идут в него.
                if (!texFile) {
                    if (s.position() + 4 <= end) {
                        s.readU16();
                        s.readU16();
                    }
                    matrixBanks.emplace_back();
                    colorBanks.emplace_back();
                }
            }
            while (s.position() < end) {
                s.readU8();
            }
        }
    }
    std::string filename;
    std::vector<std::vector<std::uint8_t>> texRaws_;
    int expectedClips = 0;
    int expectedShapes = 0;
    int expectedFields = 0;
    int expectedTextures = 0;
};
