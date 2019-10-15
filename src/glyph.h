//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_GLYPH_H
#define GLYPH_LOADING_GLYPH_H

#include "bitmap.h"
#include <stdint.h>
#include <map>

struct glyph {
    int bbox_width, bbox_hight;
    int left_side_bearing, right_side_bearing, up_side_bearing;
    int advance_width;
    bitmap<unsigned char> bmp;
    std::map<uint32_t, int> kernings;

    void scale(float factor) {
        bbox_width *= factor;
        bbox_hight *= factor;
        left_side_bearing *= factor;
        right_side_bearing *= factor;
        up_side_bearing *= factor;
        advance_width *= factor; // have to
        for (auto& p : kernings) { // have to
            p.second *= factor; // TODO: work?
        }
    }
};

#endif //GLYPH_LOADING_GLYPH_H
