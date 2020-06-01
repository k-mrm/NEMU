#include "gui/gui.h"
#include "ppu/palette.h"

void gui_init(GUI *gui) {
  al_init();
  gui->display = al_create_display(256, 240);
}

void gui_render_console(GUI *gui, Disp screen) {
  for(int y = 0; y < 240; ++y) {
    for(int x = 0; x < 256; ++x) {
      printf("%c", screen[y][x] ? '#': '.');
      //printf("%2x%2x ", y, x);
    }
    puts("");
  }
}

void gui_render(GUI *gui, Disp screen) {
  static int a = 0;
  int b = 0;
  printf("ora gokuu%d\n", a++);
  for(int y = 0; y < 240; ++y) {
    for(int x = 0; x < 256; ++x) {
      RGB rgb = colors[screen[y][x]];
      al_draw_pixel(x, y, al_map_rgb(rgb.r, rgb.g, rgb.b));
    }
  }

  al_flip_display();
}
