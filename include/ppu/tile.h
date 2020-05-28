#ifndef NEMU_PPU_TILE_H
#define NEMU_PPU_TILE_H

#include <stdint.h>

typedef uint8_t PixelPattern[8][8];

typedef struct Tile Tile;
struct Tile {
  PixelPattern pp;
  uint8_t paletteid;
};

Tile *new_tile(void);
Tile *ppu_make_tile(PPU *, uint8_t, uint8_t, uint16_t);
Tile *ppu_make_sprite_tile(PPU *, uint16_t, uint8_t);

#endif
