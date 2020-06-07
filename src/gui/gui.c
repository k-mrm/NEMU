#include "gui/gui.h"
#include "ppu/palette.h"

#define FPS 60

void gui_init(GUI *gui) {
  al_init();
  al_init_primitives_addon();
  gui->display = al_create_display(256, 240);
  gui->event_queue = al_create_event_queue();
  gui->timer = al_create_timer(1.0 / FPS);
  al_register_event_source(gui->event_queue, al_get_timer_event_source(gui->timer));
  al_start_timer(gui->timer);
}

void request_frame(GUI *gui) {
  for(;;) {
    ALLEGRO_EVENT e;
    al_wait_for_event(gui->event_queue, &e);
    if(e.type == ALLEGRO_EVENT_TIMER)
      break;
  } 
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
  static int times = 0;
  int size = 0;
  int b = 0;
  printf("ora gokuu%d\n", times++);
  for(int y = 0; y < 240; ++y) {
    for(int x = 0; x < 256; ++x) {
      RGB rgb = colors[screen[y][x]];
      gui->vtx[size].x = x;
      gui->vtx[size].y = y;
      gui->vtx[size++].color = al_map_rgb(rgb.r, rgb.g, rgb.b);
      // al_draw_pixel(x, y, al_map_rgb(rgb.r, rgb.g, rgb.b));
    }
  }

  al_draw_prim(gui->vtx, NULL, NULL, 0, 240 * 256, ALLEGRO_PRIM_POINT_LIST);
  al_flip_display();
}
