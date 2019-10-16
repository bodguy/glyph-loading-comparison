//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_GLYPH_H
#define GLYPH_LOADING_GLYPH_H

#include "bitmap.h"
#include <map>

#define FT_CEIL(X)  (((X + 63) & -64) / 64)

template<typename T>
struct vec2 {
  T x, y;
};

struct glyph_info {
  vec2<int> size;
  vec2<int> bearing;
  int advance;
  bitmap<unsigned char> bitmap;
  std::map<uint32_t, int> kerning;
};

struct font_info {
  int pixel_height;
  int ascender, descender;
  int line_gap;
};

#endif //GLYPH_LOADING_GLYPH_H
