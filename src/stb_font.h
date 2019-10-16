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

  bool open_new_face(const std::string& filename, unsigned int index) {
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

  void load_glyph_bitmap(glyph_info* glyph, int glyph_index, int pixel_height) {
    float scale = stbtt_ScaleForPixelHeight(face, pixel_height);
    unsigned char* buf = stbtt_GetGlyphBitmapSubpixel(face, scale, scale, 0.f, 0.f, glyph_index, &glyph->size.x, &glyph->size.y, nullptr, nullptr);
    bitmap<unsigned char> bmp(glyph->size.x, glyph->size.y, (unsigned char)0);

    // copy from bottom-top to top-bottom (reverse)
    for (int y = 0; y < glyph->size.y; y++) {
      for (int x = 0; x < glyph->size.x; x++) {
        int inverted_y = (glyph->size.y - 1) - y;
        unsigned char buf_byte = buf[x + inverted_y * glyph->size.x];
        bmp.set(x, y, buf_byte);
      }
    }

    free(buf);
    glyph->bitmap = bmp;
    stbtt_GetGlyphBitmapBox(face, glyph_index, scale, scale, nullptr, &glyph->bearing.y, nullptr, nullptr); // ???
    glyph->bearing.y = -glyph->bearing.y;
    load_glyph_metrics(glyph, glyph_index);
  }

  void load_glyph_metrics(glyph_info* glyph, int glyph_index) {
    stbtt_GetGlyphHMetrics(face, glyph_index, &glyph->advance, &glyph->bearing.x); // good
    int xMin, yMin, xMax, yMax;
//    stbtt_GetGlyphBox(face, glyph_index, &xMin, &yMin, &xMax, &yMax); // good
//    glyph->bbox_width = xMax - xMin;
//    glyph->bbox_height = yMax - yMin;
  }

  void print_info(const glyph_info& ginfo) {
    printf("w: %d\nh: %d\nbearing-x: %d\nbearing-y: %d\nadvance: %d\n", ginfo.size.x, ginfo.size.y, ginfo.bearing.x, ginfo.bearing.y, ginfo.advance);
    printf("asc: %d\ndesc: %d\nline_gap: %d\npixel_height: %d\n", info->ascender, info->descender, info->line_gap, info->pixel_height);
  }

  stbtt_fontinfo* face;
  font_info* info;
};


#endif //GLYPH_LOADING_STB_FONT_H
