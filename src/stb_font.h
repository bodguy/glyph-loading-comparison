//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_STB_FONT_H
#define GLYPH_LOADING_STB_FONT_H

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_font.h"
#include "glyph.h"
#include <cstdlib>
#include <string>

struct stb_font {
  stb_font() {
    face = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
    info = (font_info*)malloc(sizeof(font_info));
  }

  ~stb_font() {
    free(face);
    free(info);
  }

  bool init_font(const std::string& filename, unsigned int index) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) return false;

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * size); // no need to free
    if (!buffer) return false;
    fread(buffer, size, 1, fp);
    fclose(fp);

    if (!stbtt_InitFont(face, buffer, index)) return false;
    stbtt_GetFontVMetrics(face, &info->ascender, &info->descender, &info->line_gap);
    info->ascender = FT_CEIL(info->ascender);
    info->descender = FT_CEIL(info->descender);
    info->line_gap = FT_CEIL(info->line_gap);

    return true;
  }

  bool load_glyph(glyph_info* out_glyph_info, char codepoint, int pixel_height) {
    int glyph_index = stbtt_FindGlyphIndex(face, codepoint);
    if (glyph_index == 0) {
      return false;
    }
    info->pixel_height = pixel_height;
    load_glyph_bitmap(out_glyph_info, glyph_index, pixel_height);
    stbi_write_png("stb_tt_output.png", out_glyph_info->size.x, out_glyph_info->size.y, 1, out_glyph_info->bitmap.data.data(), out_glyph_info->size.x);

    return true;
  }

  void load_glyph_bitmap(glyph_info* out_glyph_info, int glyph_index, int pixel_height) {
    float scale = stbtt_ScaleForPixelHeight(face, pixel_height);
    unsigned char* buf = stbtt_GetGlyphBitmapSubpixel(face, scale, scale, 0.f, 0.f, glyph_index, &out_glyph_info->size.x, &out_glyph_info->size.y, nullptr, nullptr);
    bitmap<unsigned char> bmp(out_glyph_info->size.x, out_glyph_info->size.y, (unsigned char)0);

    // copy from bottom-top to top-bottom (reverse)
    for (int y = 0; y < out_glyph_info->size.y; y++) {
      for (int x = 0; x < out_glyph_info->size.x; x++) {
        int inverted_y = (out_glyph_info->size.y - 1) - y;
        unsigned char buf_byte = buf[x + inverted_y * out_glyph_info->size.x];
        bmp.set(x, y, buf_byte);
      }
    }

    free(buf);
    out_glyph_info->bitmap = bmp;
    stbtt_GetGlyphBitmapBox(face, glyph_index, scale, scale, nullptr, &out_glyph_info->bearing.y, nullptr, nullptr);
    stbtt_GetGlyphHMetrics(face, glyph_index, &out_glyph_info->advance, &out_glyph_info->bearing.x);
    out_glyph_info->advance = FT_CEIL(out_glyph_info->advance);
  }

  void print_info(const glyph_info& ginfo) {
    printf("w: %d\nh: %d\nbearing-x: %d\nbearing-y: %d\nadvance: %d\n", ginfo.size.x, ginfo.size.y, ginfo.bearing.x, ginfo.bearing.y, ginfo.advance);
    printf("asc: %d\ndesc: %d\nline_gap: %d\npixel_height: %d\n", info->ascender, info->descender, info->line_gap, info->pixel_height);
  }

  stbtt_fontinfo* face;
  font_info* info;
};


#endif //GLYPH_LOADING_STB_FONT_H
