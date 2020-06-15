#include "joypad/joypad.h"

void joypad_init(Joypad *pad) {
  pad->reg.pad1 = 0;
  pad->reg.pad2 = 0;
  pad->btnstate.pad1 = 0;
  pad->btnstate.pad2 = 0;
}

void joypad_write(Joypad *pad, int padn, uint8_t data) {
  switch(padn) {
    case 1: {
      if(pad->reg.pad1 == 1 && (data & 1) == 0) {
        pad->btnstate.pad1 = 0;
        pad->btnstate.pad2 = 0;
      }
      pad->reg.pad1 = data & 1;
      break;
    }
    default: break;
  }
}

static uint8_t read_btnstate(enum button btn) {
  ALLEGRO_KEYBOARD_STATE state;
  al_get_keyboard_state(&state);
  switch(btn) {
    case BUTTON_A: return al_key_down(&state, ALLEGRO_KEY_K);
    case BUTTON_B: return al_key_down(&state, ALLEGRO_KEY_J);
    case BUTTON_SELECT: return al_key_down(&state, ALLEGRO_KEY_I);
    case BUTTON_START: return al_key_down(&state, ALLEGRO_KEY_U);
    case BUTTON_UP: return al_key_down(&state, ALLEGRO_KEY_W);
    case BUTTON_DOWN: return al_key_down(&state, ALLEGRO_KEY_S);
    case BUTTON_LEFT: return al_key_down(&state, ALLEGRO_KEY_A);
    case BUTTON_RIGHT: return al_key_down(&state, ALLEGRO_KEY_D);
  }
  panic("nandaomae %d", btn);
}

uint8_t joypad_read(Joypad *pad, int padn) {
  switch(padn) {
    case 1: return read_btnstate(pad->btnstate.pad1++);
    case 2: return read_btnstate(pad->btnstate.pad2++);
  }
}

