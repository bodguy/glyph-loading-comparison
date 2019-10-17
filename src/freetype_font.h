//
// Created by bodguy on 2019/10/16.
//

#ifndef GLYPH_LOADING_FREETYPE_FONT_H
#define GLYPH_LOADING_FREETYPE_FONT_H

#include "glyph.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cstdlib>
#include <string>
#include <map>

struct freetype_font {
  freetype_font() {}
  ~freetype_font() {
    if (face) {
      FT_Done_Face(face);
      face = nullptr;
    }

    if (ft) {
      FT_Done_FreeType(ft);
      ft = nullptr;
    }

    for (auto& p : glyph_map) {
      delete(p.second);
      p.second = nullptr;
    }
    glyph_map.clear();
  }

  bool init_font(const char* filename, unsigned int index, int pixel_size) {
    FT_Error error = FT_Init_FreeType(&ft);
    if (error) return false;
    memset(&info, 0, sizeof(info));
    FT_New_Face(ft, filename, index, &face);
    set_pixel_height(pixel_size);
    info.ascender = face->ascender;
    info.descender = face->descender;
    info.line_gap = face->height - face->ascender + face->descender;

    return true;
  }

  void set_pixel_height(int pixel_height) {
    FT_Size_RequestRec req;
    req.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
    req.width = 0; // omit then equal to height value.
    req.height = (uint32_t)pixel_height << 6; // pixel_height is pixel size, so convert to point size (26.6 fixed floating point)
    // dpi, if 0 then 72dpi is default.
    req.horiResolution = 0;
    req.vertResolution = 0;
    FT_Request_Size(face, &req);
    info.pixel_height = pixel_height;
  }

  bool load_glyph(uint32_t codepoint) {
    uint32_t glyph_index = FT_Get_Char_Index(face, codepoint);
    if (glyph_index == 0) return false;
    FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_TARGET_NORMAL);
    if (error) return false;
    FT_GlyphSlot slot = face->glyph;
    assert(slot->format == FT_GLYPH_FORMAT_OUTLINE);

    error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
    if (error) return false;
    FT_Bitmap* ft_bitmap = &slot->bitmap;
    glyph_info* out_glyph_info = new glyph_info();
    out_glyph_info->size.x = ft_bitmap->width;
    out_glyph_info->size.y = ft_bitmap->rows;
    out_glyph_info->bearing.x = slot->bitmap_left;
    out_glyph_info->bearing.y = -slot->bitmap_top;
    out_glyph_info->advance = FT_CEIL(slot->advance.x);
    FT_Size_Metrics metrics = face->size->metrics;
    out_glyph_info->ascender = FT_CEIL(metrics.ascender);
    out_glyph_info->descender = FT_CEIL(metrics.descender);
    out_glyph_info->line_gap = FT_CEIL(metrics.height - metrics.ascender + metrics.descender);

    bitmap<unsigned char> bmp(ft_bitmap->width, ft_bitmap->rows, (unsigned char)0);

    // copy from bottom-top to top-bottom (reverse)
    for (int y = 0; y < ft_bitmap->rows; y++) {
        for (int x = 0; x < ft_bitmap->width; x++) {
            int inverted_y = (out_glyph_info->size.y - 1) - y;
            unsigned char buf_byte = ft_bitmap->buffer[x + inverted_y * ft_bitmap->pitch];
            bmp.set(x, y, buf_byte);
        }
    }
    out_glyph_info->bitmap.clear(bmp.width, bmp.height, 0);
    out_glyph_info->bitmap = bmp;
    glyph_map.insert(std::make_pair(codepoint, out_glyph_info));

    return true;
  }

  void print_info(uint32_t codepoint) {
    if (glyph_map.find(codepoint) != glyph_map.end()) {
      glyph_info* gi = glyph_map[codepoint];
      printf("========= FreeType :\n");
      printf("w: %d\nh: %d\nbearing-x: %d\nbearing-y: %d\nadvance: %d\nscaled_asc: %d\nscaled_desc: %d\nscaled_line_gap: %d\n",
             gi->size.x, gi->size.y, gi->bearing.x, gi->bearing.y, gi->advance, gi->ascender, gi->descender, gi->line_gap);
      printf("asc: %d\ndesc: %d\nline_gap: %d\npixel_height: %d\n", info.ascender, info.descender, info.line_gap, info.pixel_height);
      // print out to file
      std::string output_filename = "freetype_output_" + std::to_string(codepoint) + ".png";
      stbi_write_png(output_filename.c_str(), gi->size.x, gi->size.y, 1, gi->bitmap.data.data(), gi->size.x);
    } else {
      printf("not found codepoint: %d", codepoint);
    }
  }

  font_info info;
  FT_Face face;
  FT_Library ft;
  std::map<uint32_t, glyph_info*> glyph_map;
};

#endif //GLYPH_LOADING_FREETYPE_FONT_H
