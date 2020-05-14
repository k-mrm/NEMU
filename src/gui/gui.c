#include "gui/gui.h"
#include "ppu/palette.h"

void gui_init(GUI *gui) {
    al_init();
    gui->display = al_create_display(256, 240);
    gui->bitmap = al_create_bitmap(256, 240);
    al_set_target_bitmap(gui->bitmap);
    //al_lock_bitmap(gui->bitmap, al_get_bitmap_format(gui->bitmap), ALLEGRO_LOCK_READWRITE);
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
    for(int y = 0; y < 240; ++y) {
        for(int x = 0; x < 256; ++x) {
            RGB rgb = colors[screen[y][x]];
            al_put_pixel(x, y, al_map_rgb(rgb.r, rgb.g, rgb.b));
        }
    }

    al_set_target_bitmap(al_get_backbuffer(gui->display));
    al_draw_bitmap(gui->bitmap, 0, 0, 0);
    al_flip_display();
    al_set_target_bitmap(gui->bitmap);
}
