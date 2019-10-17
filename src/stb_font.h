//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_STB_FONT_H
#define GLYPH_LOADING_STB_FONT_H

#include "glyph.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cstdlib>
#include <cmath>
#include <map>

struct stb_font {
  stb_font() {}
  ~stb_font() {
    if (face) {
      free(face);
      face = nullptr;
    }

    if (info) {
      free(info);
      info = nullptr;
    }

    if (buffer) {
      free(buffer);
      buffer = nullptr;
    }

    for (auto& p : glyph_map) {
      delete(p.second);
      p.second = nullptr;
    }
    glyph_map.clear();
  }

  bool init_font(const char* filename, unsigned int index, int pixel_height) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return false;

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
    if (!buffer) return false;
    fread(buffer, size, 1, fp);
    fclose(fp);

    face = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
    info = (font_info*)malloc(sizeof(font_info));
    if (!stbtt_InitFont(face, buffer, index)) return false;
    stbtt_GetFontVMetrics(face, &info->ascender, &info->descender, &info->line_gap);
    info->ascender = info->ascender;
    info->descender = info->descender;
    info->line_gap = info->line_gap;
    info->pixel_height = pixel_height;

    return true;
  }

  bool load_glyph(uint32_t codepoint) {
    int glyph_index = stbtt_FindGlyphIndex(face, codepoint);
    if (glyph_index == 0) return false;

    glyph_info* out_glyph_info = new glyph_info();
    load_glyph_bitmap(out_glyph_info, glyph_index);
    glyph_map.insert(std::make_pair(codepoint, out_glyph_info));

    return true;
  }

  void load_glyph_bitmap(glyph_info* out_glyph_info, int glyph_index) {
    float scale = stbtt_ScaleForPixelHeight(face, info->pixel_height);
    unsigned char* buf = stbtt_GetGlyphBitmapSubpixel(face, scale, scale, 0.f, 0.f, glyph_index, &out_glyph_info->size.x, &out_glyph_info->size.y, nullptr, nullptr);
    float ascender, descender, line_gap;
    stbtt_GetScaledFontVMetrics(face->data, 0, info->pixel_height, &ascender, &descender, &line_gap);
    out_glyph_info->ascender = (int)std::ceil(ascender);
    out_glyph_info->descender = (int)std::ceil(descender);
    out_glyph_info->line_gap = (int)std::ceil(line_gap);
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
    out_glyph_info->bitmap.clear(bmp.width, bmp.height, 0);
    out_glyph_info->bitmap = bmp;
    stbtt_GetGlyphBitmapBox(face, glyph_index, scale, scale, nullptr, &out_glyph_info->bearing.y, nullptr, nullptr);
    stbtt_GetGlyphHMetrics(face, glyph_index, &out_glyph_info->advance, &out_glyph_info->bearing.x);
    out_glyph_info->advance = (int)std::ceil(out_glyph_info->advance * scale);
  }

  void print_info(uint32_t codepoint) {
    if (glyph_map.find(codepoint) != glyph_map.end()) {
      glyph_info* gi = glyph_map[codepoint];
      printf("========= stb_tt :\n");
      printf("w: %d\nh: %d\nbearing-x: %d\nbearing-y: %d\nadvance: %d\nscaled_asc: %d\nscaled_desc: %d\nscaled_line_gap: %d\n",
             gi->size.x, gi->size.y, gi->bearing.x, gi->bearing.y, gi->advance, gi->ascender, gi->descender, gi->line_gap);
      printf("asc: %d\ndesc: %d\nline_gap: %d\npixel_height: %d\n", info->ascender, info->descender, info->line_gap, info->pixel_height);
      // print out to file
      std::string output_filename = "stbtt_output_" + std::to_string(codepoint) + ".png";
      stbi_write_png(output_filename.c_str(), gi->size.x, gi->size.y, 1, gi->bitmap.data.data(), gi->size.x);
    } else {
      printf("not found codepoint: %d", codepoint);
    }
  }

  stbtt_fontinfo* face;
  font_info* info;
  unsigned char* buffer;
  std::map<uint32_t, glyph_info*> glyph_map;
};


#endif //GLYPH_LOADING_STB_FONT_H
