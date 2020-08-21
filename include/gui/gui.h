#ifndef NEMU_GUI_H
#define NEMU_GUI_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

typedef struct GUI GUI;
struct GUI {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_TIMER *timer;
};

typedef ALLEGRO_VERTEX Disp[240][256];
#define put_pixel(disp, py, px, rgb) disp[py][px] = (ALLEGRO_VERTEX){ \
  .x = (px), .y = (py), .color = al_map_rgb(rgb.r, rgb.g, rgb.b)  \
}

void gui_init(GUI *);
void gui_close(GUI *);
void gui_render(Disp);
int request_frame(GUI *);

#endif
