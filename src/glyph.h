//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_GLYPH_H
#define GLYPH_LOADING_GLYPH_H

#include "bitmap.h"
#include <stdint.h>
#include <map>

struct glyph {
    uint32_t charcode; // UTF-32
    float bbox_width, bbox_hight;
    float bearing_x, bearing_y;
    float advance_x;
    bitmap<unsigned char> bmp;
    std::map<uint32_t, float> kernings;
    float s0, t0, s1, t1;

    void scale(float factor) {
        advance_x *= factor;
        bearing_x *= factor;
        bearing_y *= factor;
        bbox_width *= factor;
        bbox_hight *= factor;
    }
};

#endif //GLYPH_LOADING_GLYPH_H
