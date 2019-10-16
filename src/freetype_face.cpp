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

    unsigned int codepoint = 'A';
    FT_Set_Pixel_Sizes(face, 0, 11);
    int glyph_index = FT_Get_Char_Index(face, codepoint);
    FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

  printf("codepoint: %d (good)\nglyph_index: %d (good)\nbitmap_width: %d (good)\nbitmap_height: %d (good)\nbbox_width: %d\nbbox_height: %d\nleft_side_bearing: %d\nright_side_bearing: %d\nup_side_bearing: %d (good)\nadvance_width: %d\nascent: %d (good)\ndescent: %d (good)\nline_gap: %d\n",
    codepoint,
    glyph_index,
  	face->glyph->bitmap.width,
  	face->glyph->bitmap.rows,
  	face->glyph->metrics.width,
  	face->glyph->metrics.height,
  	face->glyph->bitmap_left,
  	face->glyph->advance.x - face->glyph->bitmap_left - face->glyph->metrics.width,
  	face->glyph->bitmap_top,
  	face->glyph->advance.x,
  	face->ascender,
  	face->descender,
         face->size->metrics.height - face->size->metrics.ascender + face->size->metrics.descender);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}