//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_FONT_FACE_H
#define GLYPH_LOADING_FONT_FACE_H

#include <string>

// https://community.khronos.org/t/font-rendering-freetype-vs-signed-distance-field/103929
// https://github.com/raphm/makeglfont/blob/master/distance_map.cpp
// https://github.com/raphm/makeglfont/blob/master/makeglfont.cpp
// what is bearing, advence etc...
// https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
struct stbtt_fontinfo;
struct glyph;
class font_face {
public:
    font_face();
    ~font_face();

    bool open_new_face(const std::string& filename, unsigned int index);
    bool load_glyph(const char codepoint, int font_size, int sdf_scale);

private:
    void internal_load_glyph(glyph* glyph, int glyph_index);

private:
    stbtt_fontinfo* face;
    unsigned int face_index;
};


#endif //GLYPH_LOADING_FONT_FACE_H
