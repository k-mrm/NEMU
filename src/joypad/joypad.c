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
      if(pad->reg.pad1 == 1 && data == 0) {
        ppu->btnstate.pad1 = 0;
        ppu->btnstate.pad2 = 0;
      }
      pad->reg.pad1 = data;
      break;
    }
    default: break;
  }
}

uint8_t joypad_read(Joypad *pad, int padn) {
  switch(padn) {
    case 1: {
      pad->btnstate.pad1++;
      break;
    }
    case 2: {
      pad->btnstate.pad2++;
      break;
    }
  }
}

void joypad_pushbtn(Joypad *pad, enum button btn) {
  ;
}
