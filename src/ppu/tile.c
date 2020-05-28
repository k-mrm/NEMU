#include <stdlib.h>
#include "ppu/ppu.h"
#include "ppu/tile.h"

Tile *new_tile() {
  Tile *t = calloc(1, sizeof(Tile));
  return t;
}

void tile_dump(Tile *tile) {
  for(int i = 0; i < 8; ++i) {
    for(int j = 0; j < 8; ++j) {
      switch(tile->pp[i][j]) {
        case 0: printf("  "); break;
        case 1: printf(". "); break;
        case 2: printf("* "); break;
        case 3: printf("# "); break;
      }
    }
    puts("");
  }
}

static Tile *ppu_make_pixelpat(PPU *ppu, uint16_t sid, uint16_t baseaddr) {
  uint8_t spr[16];
  uint16_t idx = baseaddr + sid * 16;
  for(int i = 0; i < 16; ++i, ++idx) {
    spr[i] = ppubus_read(ppu->bus, idx);
  }

  Tile *tile = new_tile();
  for(int i = 0; i < 16; ++i) {
    for(int j = 0; j < 8; ++j) {
      tile->pp[i % 8][j] |= ((spr[i] & (0x80 >> j)) != 0) << (i / 8);
    }
  }

  return tile;
}

static uint16_t get_spriteid(PPU *ppu, uint8_t x, uint8_t y, uint16_t offset) {
  return ppubus_read(ppu->bus, offset + x + y * 0x20);
}

static uint8_t get_attrid(PPU *ppu, uint8_t x, uint8_t y, uint16_t offset) {
  return ppubus_read(ppu->bus, offset + 0x3c0 + x / 4 + y / 4 * 8);
}

Tile *ppu_make_sprite_tile(PPU *ppu, uint16_t sid, uint8_t pid, uint16_t base_addr) {
  Tile *tile = ppu_make_pixelpat(ppu, sid, base_addr);
  tile->paletteid = pid;
  return tile;
}

Tile *ppu_make_tile(PPU *ppu, uint8_t x, uint8_t y, uint16_t offset) {
  uint16_t sid = get_spriteid(ppu, x, y, offset);
  uint8_t aid = get_attrid(ppu, x, y, offset);
  uint8_t blockpos = x % 4 / 2 + y % 4 / 2 * 2;
  uint8_t pid = (aid >> blockpos) & 0x03;

  return ppu_make_sprite_tile(ppu, sid, pid, 0x0);
}

