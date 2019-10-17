#include "stb_font.h"
#include "freetype_font.h"

// http://www.codersnotes.com/notes/signed-distance-fields/
// https://community.khronos.org/t/font-rendering-freetype-vs-signed-distance-field/103929
// https://aras-p.info/blog/2017/02/15/Font-Rendering-is-Getting-Interesting/
// https://github.com/raphm/makeglfont/blob/master/makeglfont.cpp
// https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
// https://learnopengl.com/In-Practice/Text-Rendering
// https://github.com/justinmeiners/stb-truetype-example/blob/master/main.c
// https://gist.github.com/matthewjberger/464281d2e9101d13f0c8e931948a2f4b

int main() {
  int pixel_height = 1024;

  stb_font ff;
  ff.init_font("../res/arial.ttf", 0, pixel_height);
  ff.load_glyph('A');
  ff.print_info('A');

  freetype_font ftf;
  ftf.init_font("../res/arial.ttf", 0, pixel_height);
  ftf.load_glyph('A');
  ftf.print_info('A');
}
