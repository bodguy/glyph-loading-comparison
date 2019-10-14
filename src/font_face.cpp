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
#include <cmath>

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

    return true;
}

bool font_face::load_glyph(const char codepoint, int font_size, int sdf_scale) {
    int glyph_index = stbtt_FindGlyphIndex(face, codepoint);
    if (glyph_index == 0) {
        return false;
    }

    glyph new_glyph;
    internal_load_glyph(&new_glyph, glyph_index);
    new_glyph.charcode = codepoint;
    stbi_write_png("out.png", new_glyph.bmp.width, new_glyph.bmp.height, 1, &new_glyph.bmp.data[0], new_glyph.bmp.width);

    return true;
}

void font_face::internal_load_glyph(glyph* glyph, int glyph_index) {
    float scale_y = 1.f; // TODO
    unsigned char* buf = stbtt_GetGlyphBitmapSubpixel(face, 0.f, scale_y, 0.f, 0.f, glyph_index, &glyph->bmp.width, &glyph->bmp.height, nullptr, nullptr);
    bitmap<unsigned char> bmp(glyph->bmp.width, glyph->bmp.height, (unsigned char)0);

    // copy with inverted y
    for (int y = 0; y < glyph->bmp.height; y++) {
        for (int x = 0; x < glyph->bmp.width; x++) {
            int inverted_y = (glyph->bmp.height - 1) - y;
            unsigned char buf_byte = buf[x + inverted_y * glyph->bmp.width];
            bmp.set(x, y, buf_byte);
        }
    }

    free(buf);
    glyph->bmp = bmp;
}
