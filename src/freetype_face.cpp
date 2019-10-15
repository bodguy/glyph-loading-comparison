//
// Created by bodguy on 2019/10/15.
//

#include "freetype_face.h"
#include <iostream>

freetype_face::freetype_face() {

}

freetype_face::~freetype_face() {

}

void freetype_face::load(const std::string& filename, int index) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  FT_Face face;
  if (FT_New_Face(ft, filename.c_str(), index, &face))
  	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 20, 20);

  unsigned int codepoint = 'A';
  if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
  	std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
  }

  printf("codepoint: %d\nglyph_index: %d\nbitmap_width: %d\nbitmap_height: %d\nbbox_width: %d\nbbox_height: %d\nleft_size_bearing: %d\nright_side_bearing: %d\nup_side_bearing: %d\nadvance_width: %d\nascent: %d\ndescent: %d\nline_gap: %d\n",
    codepoint, face->glyph->glyph_index,
  	face->glyph->bitmap.width,
  	face->glyph->bitmap.rows,
  	face->bbox.xMax - face->bbox.xMin,
  	face->bbox.yMax - face->bbox.yMin,
  	face->glyph->metrics.horiBearingX,
  	face->glyph->advance.x - face->glyph->metrics.horiBearingX - face->glyph->metrics.width,
  	face->glyph->metrics.vertBearingY,
  	face->glyph->advance.x,
  	face->ascender,
  	face->descender,
  	face->size->metrics.height - face->size->metrics.ascender + face->size->metrics.descender);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}