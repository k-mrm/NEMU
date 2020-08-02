#ifndef NEMU_GUI_H
#define NEMU_GUI_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ppu/ppu.h"

typedef struct GUI GUI;
struct GUI {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_TIMER *timer;
};

void gui_init(GUI *);
void gui_close(GUI *);
void gui_render(Disp);
int request_frame(GUI *);

#endif
