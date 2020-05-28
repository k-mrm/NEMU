#include <stdlib.h>
#include <string.h>
#include "ppu/ppu.h"
#include "ppu/tile.h"
#include "ppu/palette.h"
#include "log/log.h"

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
  switch(idx) {
    case 2: {
      uint8_t stat = ppu->reg.status;
      ppu->state.addr_write_once = false;
      disable_VBlank(ppu);
      return stat;
    }
    case 7: {
      uint8_t data = ppubus_read(ppu->bus, ppu->addr);
      printf("ppuaddr read %#x\n", ppu->addr);
      ppu->addr += is_addr_inc32(ppu)? 32 : 1;
      return data;
    }
    default:
      return 0;
  }
}

void ppu_write(PPU *ppu, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 0: ppu->reg.ctrl = data; break;
    case 1: ppu->reg.mask = data; break;
    case 3: ppu->reg.oamaddr = data; break;
    case 4:
      printf("oam %#x %#x\n", ppu->reg.oamaddr, data);
      uint8_t idx = ppu->reg.oamaddr / 4;
      switch(ppu->reg.oamaddr % 4) {
        case 0: ppu->bus->oam[idx].y = data; break;
        case 1: ppu->bus->oam[idx].tileid = data; break;
        case 2: ppu->bus->oam[idx].attr = data; break;
        case 3: ppu->bus->oam[idx].x = data; break;
      }
      ppu->reg.oamaddr++;
      break;
    case 5:
      if(!ppu->state.addr_write_once)
        ppu->scrollx = data;
      else
        ppu->scrolly = data;
      ppu->state.addr_write_once ^= 1;
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

void ppu_fetch_sprite(PPU *ppu) {
  uint8_t tmps_idx = 0;
  uint8_t sprite_y = 0;
  for(int i = 0; i < 8; ++i) {
    ppu->tmp_sprite[i] = (Sprite){0, 0, 0x4, 0};
  }
  ppu->tmp_sprite_len = 0;

  for(int i = 0; i < 64; ++i, ++tmps_idx) {
    Sprite spr = ppu->bus->oam[i];
    if(tmps_idx >= 8) {
      break;
    }
    else if(is_valid_sprite(spr) && ppu->line == spr.y + 1) {
      ppu->tmp_sprite[tmps_idx].y = spr.y;
      ppu->tmp_sprite[tmps_idx].tileid = spr.tileid;
      ppu->tmp_sprite[tmps_idx].attr = spr.attr;
      ppu->tmp_sprite[tmps_idx].x = spr.x;
    }
  }

  ppu->tmp_sprite_len = tmps_idx;
}

void ppu_draw_line(PPU *ppu, Disp screen) {
  ppu_fetch_sprite(ppu);
  Tile *tile;
  uint8_t palette[4];

  if(ppu->line % 8 == 0) {
    uint8_t y = ppu->line / 8;
    /* draw background */
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

  /* draw sprite */
  for(uint8_t idx = 0; idx < ppu->tmp_sprite_len; ++idx) {
    Sprite sprite = ppu->tmp_sprite[idx];
    tile = ppu_make_sprite_tile(ppu, sprite.tileid, sprite.attr & 0x3);
    tile_dump(tile);
    for(int i = 0; i < 4; ++i) {
      palette[i] = ppubus_read(ppu->bus, 0x3f10 + tile->paletteid * 4 + i);
    }

    for(int i = 0; i < 8; ++i) {
      for(int j = 0; j < 8; ++j) {
        uint8_t c = palette[tile->pp[j][i]];
        screen[ppu->line + j][sprite.x + i] = c;
      }
    }

    free(tile);
  }
}

int ppu_step(PPU *ppu, int cyclex3, Disp screen, int *nmi) {
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
        if(ppu->line == 241) {
          enable_VBlank(ppu);
          *nmi = is_enable_NMI(ppu)? 1: 0;
        }
        ppu->line++;
        break;
      case LINE_PRERENDER:
        ppu->line = 0;
        disable_VBlank(ppu);
        return 1;
    }
  }
  return 0;
}
