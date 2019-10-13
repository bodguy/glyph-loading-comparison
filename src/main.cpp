#include <iostream>
// #include <ft2build.h>
// #include FT_FREETYPE_H
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <stdlib.h>

struct Glyph {
	int w, h, xoff, yoff;
	int advance, left_bearing;
	int ascender, descender, linegap;
	int vertical;
	unsigned char* bitmap;
	unsigned int c;
};

bool Open_New_Face(const char* filename, unsigned int index, stbtt_fontinfo* face) {
	FILE* ttf;
	ttf = fopen(filename, "rb");
	if (!ttf) return false;

	fseek(ttf, 0, SEEK_END);
	size_t size = ftell(ttf);
	fseek(ttf, 0, SEEK_SET);
	unsigned char* buffer = new unsigned char[size];
	fread(buffer, size, 1, ttf);
	fclose(ttf);

	stbtt_InitFont(face, buffer, stbtt_GetFontOffsetForIndex(buffer, index));

	return true;
}

Glyph Load_Char(stbtt_fontinfo* face, unsigned int ch, float scale) {
	Glyph glyph;
	glyph.c = ch;

	glyph.bitmap = stbtt_GetCodepointBitmap(face, 0, scale, glyph.c, &glyph.w, &glyph.h, &glyph.xoff, &glyph.yoff);
	glyph.yoff = -glyph.yoff;
	stbtt_GetCodepointHMetrics(face, glyph.c, &glyph.advance, &glyph.left_bearing);
	stbtt_GetFontVMetrics(face, &glyph.ascender, &glyph.descender, &glyph.linegap);
	glyph.vertical = glyph.ascender - glyph.descender + glyph.linegap;

	return glyph;
}

int main() {
	stbtt_fontinfo info;
	if (!Open_New_Face("../res/CelestiaMediumRedux1.5.ttf", 0, &info)) {
		printf("error");
	}
	float scale = stbtt_ScaleForPixelHeight(&info, 48.f);
	Glyph g = Load_Char(&info, 'A', scale);

	for(int i = 0; i < g.h; i++)
	{
		for(int x = 0; x < g.w; x++)
		{
			printf("%c", g.bitmap[x+i*g.w]>>5 ? '1' : ' ');
		}
		printf("\n");
	}

	printf("Character:%c\nWidth: %d\nHeight: %d\nleft bearing: %d\ntop bearing: %d\nascender: %d\ndescender: %d\nlinegap: %d\nadvance: %d\nl bearing: %d\nvertical: %d",
		g.c, g.w, g.h, g.xoff, g.yoff, g.ascender, g.descender, g.linegap, g.advance, g.left_bearing, g.vertical);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// FreeType
	//FT_Library ft;
	//// All functions return a value different than 0 whenever an error occurred
	//if (FT_Init_FreeType(&ft))
	//	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	//// Load font as face
	//FT_Face face;
	//if (FT_New_Face(ft, "CelestiaMediumRedux1.5.ttf", 0, &face))
	//	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	//// Set size to load glyphs as
	//FT_Set_Pixel_Sizes(face, 0, 15);

	//unsigned int c = 'S';
	//unsigned int c2 = 'T';
	//// Load character glyph 
	//if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	//{
	//	std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	//}

	//printf("freetype result:\n");
	//printf("Width: %d\nHeight: %d\nAdvance: %d\nBearingX: %d\nBearingY: %d\n",
	//	face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->advance.x, 
	//	face->glyph->bitmap_left, face->glyph->bitmap_top);

	//for(unsigned int i = 0; i < face->glyph->bitmap.rows; i++)
	//{
	//	for(unsigned int x = 0; x < face->glyph->bitmap.width; x++)
	//	{
	//		printf("%c", face->glyph->bitmap.buffer[x+i*face->glyph->bitmap.width]>>5 ? '1' : ' ');
	//	}
	//	printf("\n");
	//}

	//// Destroy FreeType once we're finished
	//FT_Done_Face(face);
	//FT_Done_FreeType(ft);

	return 0;
}