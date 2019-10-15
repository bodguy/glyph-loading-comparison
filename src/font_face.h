//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_FONT_FACE_H
#define GLYPH_LOADING_FONT_FACE_H

#include <string>

// https://community.khronos.org/t/font-rendering-freetype-vs-signed-distance-field/103929
// https://aras-p.info/blog/2017/02/15/Font-Rendering-is-Getting-Interesting/
// https://github.com/raphm/makeglfont/blob/master/makeglfont.cpp
// https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
// https://learnopengl.com/In-Practice/Text-Rendering
// https://github.com/justinmeiners/stb-truetype-example/blob/master/main.c
// https://gist.github.com/matthewjberger/464281d2e9101d13f0c8e931948a2f4b
// https://fontdrop.info/
struct stbtt_fontinfo;
struct glyph;
class font_face {
public:
    font_face();
    ~font_face();

    bool open_new_face(const std::string& filename, unsigned int index);
    bool load_glyph(const char codepoint, float font_size);

private:
    void load_glyph_bitmap(glyph* glyph, int glyph_index, float pixels);
    void load_glyph_metrics(glyph* glyph, int glyph_index);

private:
    stbtt_fontinfo* face;
    unsigned int face_index;
    int ascent, descent, line_gap;
};


#endif //GLYPH_LOADING_FONT_FACE_H
