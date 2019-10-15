//
// Created by bodguy on 2019-10-14.
//

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "font_face.h"
#include "glyph.h"
#include <cstdlib>

font_face::font_face() : face_index(0) {
    face = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
}

font_face::~font_face() {
    free(face);
}

bool font_face::open_new_face(const std::string& filename, unsigned int index) {
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
    face_index = index;
    stbtt_GetFontVMetrics(face, &ascent, &descent, &line_gap);

    return true;
}

bool font_face::load_glyph(const char codepoint, float font_size) {
    int glyph_index = stbtt_FindGlyphIndex(face, codepoint);
    if (glyph_index == 0) {
        return false;
    }

    glyph new_glyph;
    load_glyph_bitmap(&new_glyph, glyph_index, font_size);
//    stbi_write_png("out.png", new_glyph.bmp.width, new_glyph.bmp.height, 1, &new_glyph.bmp.data[0], new_glyph.bmp.width);

    return true;
}

void font_face::load_glyph_bitmap(glyph* glyph, int glyph_index, float pixels) {
    float scale = stbtt_ScaleForPixelHeight(face, pixels);
    unsigned char* buf = stbtt_GetGlyphBitmapSubpixel(face, scale, scale, 0.f, 0.f, glyph_index, &glyph->bmp.width, &glyph->bmp.height, nullptr, nullptr);
    bitmap<unsigned char> bmp(glyph->bmp.width, glyph->bmp.height, (unsigned char)0);

    // copy from bottom-top to top-bottom (reverse)
    for (int y = 0; y < glyph->bmp.height; y++) {
        for (int x = 0; x < glyph->bmp.width; x++) {
            int inverted_y = (glyph->bmp.height - 1) - y;
            unsigned char buf_byte = buf[x + inverted_y * glyph->bmp.width];
            bmp.set(x, y, buf_byte);
        }
    }

    free(buf);
    glyph->bmp = bmp;
    stbtt_GetGlyphBitmapBox(face, glyph_index, scale, scale, nullptr, &glyph->up_side_bearing, nullptr, nullptr); // ???
    glyph->up_side_bearing = std::abs(glyph->up_side_bearing); // up side bearing is inverted (negative value) but we use it positive value
    load_glyph_metrics(glyph, glyph_index);
}

void font_face::load_glyph_metrics(glyph* glyph, int glyph_index) {
    stbtt_GetGlyphHMetrics(face, glyph_index, &glyph->advance_width, &glyph->left_side_bearing); // good
    int xMin, yMin, xMax, yMax;
    stbtt_GetGlyphBox(face, glyph_index, &xMin, &yMin, &xMax, &yMax); // good
    glyph->bbox_width = xMax - xMin;
    glyph->bbox_hight = yMax - yMin;
    glyph->right_side_bearing = glyph->advance_width - glyph->left_side_bearing - glyph->bbox_width; // good
}
