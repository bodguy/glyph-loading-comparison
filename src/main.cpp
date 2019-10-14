#include "font_face.h"

//struct Glyph {
//	int w, h, xoff, yoff;
//	int advance, left_bearing;
//	int ascender, descender, linegap;
//	int vertical;
//	unsigned char* bitmap; // must be freed
//	unsigned int c;
//};
//
//#define MAXUNICODE	0x10FFFF
//
//int utf8_decode(const char *o) {
//    static const unsigned int limits[] = {0xFF, 0x7F, 0x7FF, 0xFFFF};
//    const unsigned char *s = (const unsigned char *)o;
//    unsigned int c = s[0];
//    unsigned int res = 0;  /* final result */
//    if (c < 0x80)  /* ascii? */
//        res = c;
//    else {
//        int count = 0;  /* to count number of continuation bytes */
//        // 0x40: 64, 0xC0: 192, 0x80: 128
//        while (c & 0x40) {  /* still have continuation bytes? */
//            int cc = s[++count];  /* read next byte */
//            if ((cc & 0xC0) != 0x80)  /* not a continuation byte? */
//                return -1;  /* invalid byte sequence */
//            res = (res << 6) | (cc & 0x3F);  /* add lower 6 bits from cont. byte */
//            c <<= 1;  /* to test next bit */
//        }
//        res |= ((c & 0x7F) << (count * 5));  /* add first byte */
//        if (count > 3 || res > MAXUNICODE || res <= limits[count])
//            return -1;  /* invalid byte sequence */
//        s += count;  /* skip continuation bytes read */
//    }
//    return res;
//}
//
//bool Open_New_Face(const char* filename, unsigned int index, stbtt_fontinfo* face) {
//	FILE* ttf;
//	ttf = fopen(filename, "rb");
//	if (!ttf) return false;
//
//	fseek(ttf, 0, SEEK_END);
//	size_t size = ftell(ttf);
//	fseek(ttf, 0, SEEK_SET);
//	unsigned char* buffer = new unsigned char[size];
//	fread(buffer, size, 1, ttf);
//	fclose(ttf);
//
//	stbtt_InitFont(face, buffer, stbtt_GetFontOffsetForIndex(buffer, index));
//
//	return true;
//}
//
//Glyph Load_Char(stbtt_fontinfo* face, unsigned int ch, float scale) {
//	Glyph glyph;
//	glyph.c = ch;
//
//	glyph.bitmap = stbtt_GetCodepointBitmap(face, 0, scale, glyph.c, &glyph.w, &glyph.h, &glyph.xoff, &glyph.yoff);
//	glyph.yoff = -glyph.yoff;
//	stbtt_GetCodepointHMetrics(face, glyph.c, &glyph.advance, &glyph.left_bearing);
//	stbtt_GetFontVMetrics(face, &glyph.ascender, &glyph.descender, &glyph.linegap);
//	glyph.vertical = glyph.ascender - glyph.descender + glyph.linegap;
//
//	return glyph;
//}
//
//void LoadCharacters(stbtt_fontinfo* face, std::map<char, Glyph>& char_map, float scale) {
//  // A ~ Z
//  for (int i = 65; i <= 90; i++) {
//    Glyph g = Load_Char(face, i, scale);
//    char_map.insert(std::make_pair(i, g));
//  }
//
//  // a ~ z
//  for (int i = 97; i <= 122; i++) {
//    Glyph g = Load_Char(face, i, scale);
//    char_map.insert(std::make_pair(i, g));
//  }
//}
//
//void renderChar(const Glyph& g) {
//  for(int i = 0; i < g.h; i++) {
//    for(int x = 0; x < g.w; x++) {
//      printf("%c", g.bitmap[x+i*g.w]>>5 ? 'x' : ' ');
//    }
//    printf("\n");
//  }
//}
//
//void renderStr(const std::string& str, std::map<char, Glyph>& glyph_map) {
//  for (auto ch : str) {
//    if (glyph_map.find(ch) == glyph_map.end()) {
//      continue;
//    }
//    Glyph g = glyph_map[ch];
//    renderChar(g);
//  }
//}

// for signed distance field implementation reference:
// http://www.codersnotes.com/notes/signed-distance-fields/
int main() {
//    stbtt_fontinfo info;
//    if (!Open_New_Face("../res/CelestiaMediumRedux1.5.ttf", 0, &info)) {
//        printf("error");
//    }
//    float scale = stbtt_ScaleForPixelHeight(&info, 48.f);
//    std::map<char, Glyph> glyph_map;
//    LoadCharacters(&info, glyph_map, scale);
//    renderStr("Hello world", glyph_map);
//    // free
//    for (auto pair : glyph_map) {
//        free(pair.second.bitmap);
//    }
//	return 0;
/* load font file */

    font_face loader;
    bool result = loader.open_new_face("../res/arial.ttf", 0);
    if (!result) {
        fprintf(stderr, "error to open ttf file");
        return -1;
    }
    result = loader.load_glyph('A', 0, 16);
    if (!result) {
        fprintf(stderr, "error to load glyph");
        return -1;
    }
}
