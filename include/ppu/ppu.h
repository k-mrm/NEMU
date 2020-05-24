#ifndef NEMU_PPU_H
#define NEMU_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "ppu/bus.h"
#include "ppu/palette.h"

typedef struct PPU PPU;
struct PPU {
  struct PPUreg reg;
  struct {
    bool addr_write_once;
  } state;
  uint16_t addr;
  uint16_t line;
  PPUBus *bus;
  uint16_t scrollx;
  uint16_t scrolly;
  uint16_t cpu_cycle;
  int vblank;
};

typedef uint8_t Disp[240][256];

void ppu_run(PPU *, int);
void ppu_render(PPU *, Disp);
int ppu_step(PPU *, int, Disp, int *);
void ppu_init(PPU *, PPUBus *);
uint8_t ppu_read(PPU *, uint16_t);
void ppu_write(PPU *, uint16_t, uint8_t);

#endif
