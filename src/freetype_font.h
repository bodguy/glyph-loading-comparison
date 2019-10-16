//
// Created by bodguy on 2019/10/16.
//

#ifndef GLYPH_LOADING_FREETYPE_FONT_H
#define GLYPH_LOADING_FREETYPE_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "glyph.h"
#include <cstdlib>

struct freetype_font {
  freetype_font(const char* filename, int pixel_size) { init_font(filename, pixel_size); }
  ~freetype_font() { close_font(); }

  bool init_font(const char* filename, int pixel_size) {
    FT_Error error = FT_Init_FreeType(&ft);
    if (error) return false;
    memset(&info, 0, sizeof(info));
    FT_New_Face(ft, filename, 0, &face);
    set_pixel_height(pixel_size);

    return true;
  }

  void close_font() {
    if (face) {
      FT_Done_Face(face);
      face = nullptr;
    }

    if (ft) {
      FT_Done_FreeType(ft);
      ft = nullptr;
    }
  }

  void set_pixel_height(int pixel_height) {
    FT_Size_RequestRec req;
    req.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
    req.width = 0;
    req.height = (uint32_t)pixel_height * 64;
    req.horiResolution = 0;
    req.vertResolution = 0;
    FT_Request_Size(face, &req);
    FT_Size_Metrics metrics = face->size->metrics;
    info.pixel_height = pixel_height;
    info.ascender = metrics.ascender;
    info.descender = metrics.descender;
    info.line_gap = metrics.height - metrics.ascender + metrics.descender;
  }

  const FT_Glyph_Metrics* load_glyph(uint32_t codepoint) {
    uint32_t glyph_index = FT_Get_Char_Index(face, codepoint);
    if (glyph_index == 0) return nullptr;
    FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_TARGET_NORMAL);
    if (error) return nullptr;
    FT_GlyphSlot slot = face->glyph;
    assert(slot->format == FT_GLYPH_FORMAT_OUTLINE);
    return &slot->metrics;
  }

  bool render_glyph_and_get_info(glyph_info* out_glyph_info) {
    FT_GlyphSlot slot = face->glyph;
    FT_Error error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
    if (error) return false;
    FT_Bitmap* ft_bitmap = &slot->bitmap;
    out_glyph_info->size.x = ft_bitmap->width;
    out_glyph_info->size.y = ft_bitmap->rows;
    out_glyph_info->bearing.x = slot->bitmap_left;
    out_glyph_info->bearing.y = -slot->bitmap_top;
    out_glyph_info->advance = slot->advance.x;

    bitmap<unsigned char> bmp(ft_bitmap->width, ft_bitmap->rows, (unsigned char)0);

    // copy from bottom-top to top-bottom (reverse)
    for (int y = 0; y < ft_bitmap->rows; y++) {
      for (int x = 0; x < ft_bitmap->width; x++) {
        int inverted_y = (out_glyph_info->size.y - 1) - y;
        unsigned char buf_byte = ft_bitmap->buffer[x + inverted_y * ft_bitmap->pitch];
        bmp.set(x, y, buf_byte);
      }
    }
    out_glyph_info->bitmap = bmp;
    stbi_write_png("freetype_output.png", out_glyph_info->size.x, out_glyph_info->size.y, 1, out_glyph_info->bitmap.data.data(), out_glyph_info->size.x);

    return true;
  }

  void print_info(const glyph_info& glyph_info) {
    printf("w: %d\nh: %d\nbearing-x: %d\nbearing-y: %d\nadvance: %d\n", glyph_info.size.x, glyph_info.size.y, glyph_info.bearing.x, glyph_info.bearing.y, glyph_info.advance);
    printf("asc: %d\ndesc: %d\nline_gap: %d\npixel_height: %d\n", info.ascender, info.descender, info.line_gap, info.pixel_height);
  }

  font_info info;
  FT_Face face;
  FT_Library ft;
};

#endif //GLYPH_LOADING_FREETYPE_FONT_H
