#ifndef NEMU_PPU_BUS_H
#define NEMU_PPU_BUS_H

#include <stdint.h>
#include "ppu/sprite.h"
#include "cassette/cassette.h"

typedef struct PPUBus PPUBus;
struct PPUBus {
  Cassette *cassette;
  uint8_t *vram;
  uint8_t *palette;
  uint8_t *oam;
};

uint8_t ppubus_read(PPUBus *, uint16_t);
void ppubus_write(PPUBus *, uint16_t, uint8_t);
void ppubus_init(PPUBus *, Cassette *);

#define is_valid_sprite(sprite) (!((sprite).attr & 0x04))

#endif
