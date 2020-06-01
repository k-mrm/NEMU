#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ppu/ppu.h"

typedef struct GUI GUI;
struct GUI {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
};

void gui_init(GUI *);
void gui_render(GUI *, Disp);
void gui_render_console(GUI *, Disp);
