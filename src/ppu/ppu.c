#include <stdlib.h>
#include <string.h>
#include "ppu/ppu.h"
#include "ppu/tile.h"
#include "ppu/palette.h"
#include "log/log.h"

#define put_pixel(disp, py, px, rgb) disp[py][px] = (ALLEGRO_VERTEX){ \
  .x = (px), .y = (py), .color = al_map_rgb(rgb.r, rgb.g, rgb.b)  \
}

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
  switch(idx) {
    case 2: {
      uint8_t status = ppu->reg.status;
      ppu->state.addr_write_once = false;
      disable_VBlank(ppu);
      return status;
    }
    case 7: {
      uint8_t data = ppubus_read(ppu->bus, ppu->addr);
      // printf("ppuaddr read %#x\n", ppu->addr);
      ppu->addr += is_addr_inc32(ppu)? 32: 1;
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
    case 4: {
      uint8_t idx = ppu->reg.oamaddr / 4;
      switch(ppu->reg.oamaddr % 4) {
        case 0: ppu->bus->oam[idx].y = data; break;
        case 1: ppu->bus->oam[idx].tileid = data; break;
        case 2: ppu->bus->oam[idx].attr = data; break;
        case 3: ppu->bus->oam[idx].x = data; break;
      }
      ppu->reg.oamaddr++;
      break;
    }
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
      // printf("ppuaddr write %#x\n", ppu->addr);
      ppu->addr += is_addr_inc32(ppu)? 32: 1;
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

  memset(ppu->priority, 0, sizeof(uint8_t) * 240 * 256);
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

  for(int i = 0; i < 64; ++i) {
    Sprite spr = ppu->bus->oam[i];
    if(tmps_idx >= 8) {
      break;
    }
    else if(spr.y + 1 <= ppu->line && ppu->line < spr.y + 1 + 8) {
      ppu->tmp_sprite[tmps_idx] = spr;
      ++tmps_idx;
    }
  }

  ppu->tmp_sprite_len = tmps_idx;
}

uint16_t nametable_addr(PPU *ppu) {
  uint16_t f = ppu->reg.ctrl & 0x03;
  return 0x2000 + f * 0x400;
}

uint16_t bg_pattable_addr(PPU *ppu) {
  uint16_t f = (ppu->reg.ctrl >> 4) & 0x01;
  return f * 0x1000;
}

uint16_t sprite_pattable_addr(PPU *ppu) {
  uint16_t f = (ppu->reg.ctrl >> 3) & 0x01;
  return f * 0x1000;
}

void ppu_draw_line(PPU *ppu, Disp screen) {
  ppu_fetch_sprite(ppu);
  Tile *tile;
  uint8_t palette[4];

  if(ppu->line % 8 == 0) {
    uint8_t y = ppu->line / 8;
    /* draw background */
    for(uint8_t x = 0; x < 32; x++) {
      tile = ppu_make_bg_tile(ppu, x, y, nametable_addr(ppu), bg_pattable_addr(ppu));
      // tile_dump(tile);
      for(int i = 0; i < 4; ++i) {
        palette[i] = ppubus_read(ppu->bus, 0x3f00 + tile->paletteid * 4 + i);
        // printf("palette %d = %d ", i, palette[i]);
      }

      for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
          /*
          printf("%02x", c);
          if(tile->pp[j][i])
           printf("at %#x %#x\n", x * 8 + i, ppu->line + j);
          */
          uint8_t cidx = tile->pp[j][i];
          RGB rgb = colors[palette[cidx]];
          put_pixel(screen, ppu->line + j, x * 8 + i, rgb);
        }
      }

      free(tile);
    }
    //puts("");
  }

  /* draw sprite */
  for(uint8_t idx = 0; idx < ppu->tmp_sprite_len; ++idx) {
    Sprite sprite = ppu->tmp_sprite[idx];
    tile = ppu_make_sprite_tile(ppu, sprite.tileid, sprite.attr & 0x3, sprite_pattable_addr(ppu));
    // tile_dump(tile);
    for(int i = 0; i < 4; ++i) {
      palette[i] = ppubus_read(ppu->bus, 0x3f10 + tile->paletteid * 4 + i);
    }

    for(int i = 0; i < 8; ++i) {
      uint8_t cidx = tile->pp[ppu->line - (sprite.y + 1)][i];
      if(cidx != 0) {
        RGB rgb = colors[palette[cidx]];
        put_pixel(screen, ppu->line, sprite.x + i, rgb);
      }
    }

    free(tile);
  }
}

int ppu_step(PPU *ppu, int cyclex3, Disp screen, int *nmi) {
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
        *nmi = is_enable_nmi(ppu)? 1: 0;
      }
      ppu->line++;
      break;
    case LINE_PRERENDER:
      ppu->line = 0;
      disable_VBlank(ppu);
      memset(ppu->priority, 0, sizeof(uint8_t) * 240 * 256);
      return 1;
  }
  return 0;
}
