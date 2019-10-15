//
// Created by bodguy on 2019/10/15.
//

#ifndef GLYPH_LOADING_FREETYPE_FACE_H
#define GLYPH_LOADING_FREETYPE_FACE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>

class freetype_face {
public:
  freetype_face();
  ~freetype_face();

  void load(const std::string& filename, int index);

private:
};


#endif //GLYPH_LOADING_FREETYPE_FACE_H
