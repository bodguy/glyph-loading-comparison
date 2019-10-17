#include "stb_font.h"
#include "freetype_font.h"

#define MAXUNICODE	0x10FFFF

int utf8_decode(const char *o) {
  static const unsigned int limits[] = {0xFF, 0x7F, 0x7FF, 0xFFFF};
  const unsigned char *s = (const unsigned char *)o;
  unsigned int c = s[0];
  unsigned int res = 0;  /* final result */
  if (c < 0x80)  /* ascii? */
    res = c;
  else {
    int count = 0;  /* to count number of continuation bytes */
    while (c & 0x40) {  /* still have continuation bytes? */
      int cc = s[++count];  /* read next byte */
      if ((cc & 0xC0) != 0x80)  /* not a continuation byte? */
        return -1;  /* invalid byte sequence */
      res = (res << 6) | (cc & 0x3F);  /* add lower 6 bits from cont. byte */
      c <<= 1;  /* to test next bit */
    }
    res |= ((c & 0x7F) << (count * 5));  /* add first byte */
    if (count > 3 || res > MAXUNICODE || res <= limits[count])
      return -1;  /* invalid byte sequence */
    s += count;  /* skip continuation bytes read */
  }
  return res;
}

int main() {
  const char* font_file = "../res/NanumGothicCoding.ttf";
  int pixel_height = 48;
  char* codepoint = "푸";
  int cp = utf8_decode(codepoint);

  stb_font ff;
  ff.init_font(font_file, 0, pixel_height);
  ff.load_glyph(cp);
  ff.print_info(cp);

  freetype_font ftf;
  ftf.init_font(font_file, 0, pixel_height);
  ftf.load_glyph(cp);
  ftf.print_info(cp);
}
