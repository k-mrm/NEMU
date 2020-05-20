#ifndef NEMU_PPU_SPRITE_H
#define NEMU_PPU_SPRITE_H

#include <stdint.h>

typedef struct Sprite Sprite;
struct Sprite {
  uint8_t y;
  uint8_t tileid;
  uint8_t attr;
  uint8_t x;
};

#endif
