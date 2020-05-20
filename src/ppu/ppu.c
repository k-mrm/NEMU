#include <stdlib.h>
#include "ppu/ppu.h"
#include "ppu/tile.h"
#include "ppu/palette.h"

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
  switch(idx) {
    case 7: {
      uint8_t data = ppubus_read(ppu->bus, ppu->addr);
      printf("ppuaddr read %#x\n", ppu->addr);
      ppu->addr++;
      return data;
    }
    default:
      return 0;
  }
}

void ppu_write(PPU *ppu, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 3:
      ppu->reg.oamaddr = data;
      break;
    case 4:
      ppu->bus->oam[ppu->reg.oamaddr] = data;
      ppu->reg.oamaddr++;
      break;
    case 6:
      ppu->addr = ppu->state.addr_write_once ?
        ppu->addr | data :
        (uint16_t)data << 8;
      ppu->state.addr_write_once ^= 1;
      break;
    case 7:
      ppubus_write(ppu->bus, ppu->addr, data);
      printf("ppuaddr write %#x\n", ppu->addr);
      ppu->addr++;
      break;
    default:
      break;
  }
}

void ppu_init(PPU *ppu, PPUBus *bus) {
  ppu->reg.ctrl = 0;
  ppu->reg.mask = 0;
  ppu->reg.status = 0;
  ppu->reg.oamaddr = 0;
  ppu->reg.scroll = 0;
  ppu->reg.addr = 0;
  ppu->reg.data = 0;
  ppu->reg.oamdma = 0;

  ppu->state.addr_write_once = false;

  ppu->addr = 0;
  ppu->line = 0;
  ppu->bus = bus;
  ppu->cpu_cycle = 0;
}

enum {
  LINE_VISIBLE,
  LINE_POSTRENDER,
  LINE_VERTICAL_BLANKING,
  LINE_PRERENDER,
};

int linestate_from(uint16_t line) {
  if(line < 240) {
    return LINE_VISIBLE;
  }
  else if(line == 240) {
    return LINE_POSTRENDER;
  }
  else if(line < 261) {
    return LINE_VERTICAL_BLANKING;
  }
  else if(line == 261) {
    return LINE_PRERENDER;
  }
  else {
    panic("invalid line");
    return -1;
  }
}

void ppu_draw_line(PPU *ppu, Disp screen) {
  if(ppu->line % 8 == 0) {
    Tile *tile;
    uint8_t palette[4];
    uint8_t y = ppu->line / 8;
    for(uint8_t x = 0; x < 32; x++) {
      tile = ppu_make_tile(ppu, x, y, 0x2000);
      for(int i = 0; i < 4; ++i) {
        palette[i] = ppubus_read(ppu->bus, 0x3f00 + tile->paletteid * 4 + i);
      }

      for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
          uint8_t c = palette[tile->pp[j][i]];
          /* printf("%02x", c);
             if(tile->pp[j][i])
             printf("at %#x %#x\n", x * 8 + i, ppu->line + j);
             */
          screen[ppu->line + j][x * 8 + i] = c;
        }
      }

      free(tile);
    }
    //puts("");
  }

  /* TODO: draw sprite */
}

int ppu_step(PPU *ppu, int cyclex3, Disp screen) {
  ppu->cpu_cycle += cyclex3;
  if(ppu->cpu_cycle >= 341) {
    ppu->cpu_cycle -= 341;

    switch(linestate_from(ppu->line)) {
      case LINE_VISIBLE:
        ppu_draw_line(ppu, screen);
        ppu->line++;
        break;
      case LINE_POSTRENDER:
        ppu->line++;
        break;
      case LINE_VERTICAL_BLANKING:
        ppu->line++;
        break;
      case LINE_PRERENDER:
        ppu->line = 0;
        return 1;
    }
  }
  return 0;
}
