#include "gui/gui.h"
#include "ppu/palette.h"

#define FPS 60

void gui_init(GUI *gui) {
  al_init();
  al_init_primitives_addon();
  al_install_keyboard();
  gui->display = al_create_display(256, 240);
  gui->event_queue = al_create_event_queue();
  gui->timer = al_create_timer(1.0 / FPS);
  al_register_event_source(gui->event_queue, al_get_timer_event_source(gui->timer));
  al_register_event_source(gui->event_queue, al_get_display_event_source(gui->display));
  al_start_timer(gui->timer);
}

void gui_close(GUI *gui) {
  al_destroy_display(gui->display);
  al_destroy_event_queue(gui->event_queue);
  al_destroy_timer(gui->timer);
}

int request_frame(GUI *gui) {
  for(;;) {
    ALLEGRO_EVENT e;
    al_wait_for_event(gui->event_queue, &e);
    if(e.type == ALLEGRO_EVENT_TIMER)
      return 1;
    else if(e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      return 0;
  } 
}

void gui_render(Disp screen) {
  al_draw_prim(screen, NULL, NULL, 0, 240 * 256, ALLEGRO_PRIM_POINT_LIST);
  al_flip_display();
}
