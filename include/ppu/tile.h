#ifndef NEMU_PPU_TILE_H
#define NEMU_PPU_TILE_H

#include <stdint.h>

typedef uint8_t PixelPattern[8][8];

typedef struct Tile Tile;
struct Tile {
  PixelPattern pp;
  uint8_t paletteid;
};

void ppu_make_bg_tile(PPU *, Tile *, uint8_t, uint8_t, uint16_t, uint16_t);
void ppu_make_sprite_tile(PPU *, Tile *, uint16_t, uint8_t, uint8_t, uint16_t);
void tile_dump(Tile *);

#endif
