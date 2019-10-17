### glyph-loading

brief comparision with stb_truetype and freetype.  
THIS IS NOT A PRODUCTION LEVEL CODE.

### debian distro

for using freetype, install it.

```bash
sudo apt-get install libfreetype6-dev
```

### vagrant

vagrant test pass

### simple code

```c++
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
```

### outputs

glyph bitmap images are dropped on executable directory.

```
========= stb_tt :
w: 615
h: 657
bearing-x: -3
bearing-y: -657
advance: 612
scaled_asc: 830
scaled_desc: -194
scaled_line_gap: 30
asc: 1854
desc: -434
line_gap: 67
pixel_height: 1024
========= FreeType :
w: 615
h: 657
bearing-x: -2
bearing-y: -657
advance: 612
scaled_asc: 830
scaled_desc: -195
scaled_line_gap: 29
asc: 1854
desc: -434
line_gap: 67
pixel_height: 1024
```

- stb_truetype output
![stb_truetype](./stbtt_output_65.png)   
- freetype output
![freetype](./freetype_output_65.png)