#include "gui/gui.h"
#include "ppu/palette.h"

void gui_init(GUI *gui) {
    al_init();
    gui->display = al_create_display(256, 240);
    gui->bitmap = al_create_bitmap(256*2, 240*2);
    al_set_target_bitmap(gui->bitmap);
    al_lock_bitmap(gui->bitmap, al_get_bitmap_format(gui->bitmap), ALLEGRO_LOCK_READWRITE);
}

void gui_render_console(GUI *gui, Disp screen) {
    for(int i = 0; i < 256; ++i) {
        for(int j = 0; j < 240; ++j) {
            printf("%02x", screen[i][j]);
        }
        puts("");
    }
}

void gui_render(GUI *gui, Disp screen) {
    for(int i = 0; i < 256; ++i) {
        for(int j = 0; j < 240; ++j) {
            RGB rgb = colors[screen[i][j]];
            al_put_pixel(i, j, al_map_rgb(rgb.r, rgb.g, rgb.b));
        }
    }

    al_set_target_bitmap(al_get_backbuffer(gui->display));
    al_draw_bitmap(gui->bitmap, 256, 240, 0);
    al_flip_display();
    al_set_target_bitmap(gui->bitmap);
}
