#include <stdlib.h>
#include <string.h>
#include "ppu/ppu.h"
#include "ppu/tile.h"
#include "ppu/palette.h"
#include "log/log.h"
#include "gui/gui.h"

#define put_pixel(disp, py, px, rgb) disp[py][px] = (ALLEGRO_VERTEX){ \
  .x = (px), .y = (py), .color = al_map_rgb(rgb.r, rgb.g, rgb.b)  \
}
#define enable_VBlank(ppu)  (ppu)->io.status |= (1 << 7)
#define disable_VBlank(ppu) (ppu)->io.status &= ~(1 << 7)

#define sprite_0hit(ppu)    (ppu)->io.status |= (1 << 6)

#define is_enable_nmi(ppu)  ((ppu)->io.ctrl & (1 << 7))
#define is_addr_inc32(ppu)  ((ppu)->io.ctrl & (1 << 2))

#define is_enable_bg(ppu)     ((ppu)->io.mask & (1 << 3))
#define is_enable_sprite(ppu) ((ppu)->io.mask & (1 << 4))

uint8_t ppu_vramaddr_inc(PPU *ppu) {
  return is_addr_inc32(ppu)? 32: 1;
}

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
  // log_dbg("ppu_read %u\n", idx);
  uint8_t res;
  switch(idx) {
    case 2: {
      uint8_t status = ppu->io.status;
      ppu->write_once = false;
      disable_VBlank(ppu);
      res = status;
      break;
    }
    case 7: {
      uint8_t data = ppubus_read(ppu->bus, ppu->vramaddr);
      // printf("ppuaddr read %#x\n", ppu->addr);
      ppu->vramaddr += ppu_vramaddr_inc(ppu);
      res = data;
      break;
    }
    default:
      res = 0;
      break;
  }

  return res;
}

void ppu_write(PPU *ppu, uint16_t idx, uint8_t data) {
  // log_dbg("ppu_write %u <- %u\n", idx, data);
  switch(idx) {
    case 0: ppu->io.ctrl = data; break;
    case 1: ppu->io.mask = data; break;
    case 3: ppu->io.oamaddr = data; break;
    case 4: ppu_oam_write(ppu, data); break;
    case 5:
      if(!ppu->write_once)
        ppu->scrollx = data;
      else
        ppu->scrolly = data;
      ppu->write_once ^= 1;
      break;
    case 6:
      ppu->vramaddr = ppu->write_once?
        ppu->vramaddr | data :
        (uint16_t)data << 8;
      ppu->write_once ^= 1;
      break;
    case 7:
      ppubus_write(ppu->bus, ppu->vramaddr, data);
      // printf("ppuaddr write %#x\n", ppu->addr);
      ppu->vramaddr += ppu_vramaddr_inc(ppu);
      break;
    default:
      break;
  }
}

void ppu_init(PPU *ppu, PPUBus *bus) {
  ppu->io.ctrl = 0;
  ppu->io.mask = 0;
  ppu->io.status = 0;
  ppu->io.oamaddr = 0;
  ppu->io.scroll = 0;
  ppu->io.addr = 0;
  ppu->io.data = 0;
  ppu->io.oamdma = 0;

  ppu->write_once = false;

  ppu->vramaddr = 0;
  ppu->line = 0;
  ppu->bus = bus;
  ppu->cpu_cycle = 0;
  ppu->scrollx = 0;
  ppu->scrolly = 0;
}

enum linestate {
  VISIBLE,
  POSTRENDER,
  VERTICAL_BLANKING,
  PRERENDER,
};

static enum linestate linestate_from(uint16_t line) {
  log_dbg("scanline %u\n", line);
  if(line < 240) return VISIBLE;
  else if(line == 240) return POSTRENDER;
  else if(line < 261) return VERTICAL_BLANKING;
  else if(line == 261) return PRERENDER;

  panic("invalid line");
  return 0;
}

static void ppu_fetch_sprite(PPU *ppu) {
  if(!is_enable_sprite(ppu)) return;

  uint8_t tmps_idx = 0;
  uint8_t sprite_y = 0;
  ppu->tmp_sprite_len = 0;

  for(int i = 0; i < 64; ++i) {
    uint8_t y = ppu->bus->oam[i * 4];
    uint8_t tileid = ppu->bus->oam[i * 4 + 1];
    uint8_t attr = ppu->bus->oam[i * 4 + 2];
    uint8_t x = ppu->bus->oam[i * 4 + 3];
    Sprite spr = (Sprite){
      .y = y, .tileid = tileid, .attr = attr, .x = x,
    };
    if(tmps_idx >= 8) {
      break;
    }
    else if(spr.y + 1 <= ppu->line && ppu->line < spr.y + 1 + 8) {
      if(i == 0) {
        sprite_0hit(ppu);
      }
      ppu->tmp_sprite[tmps_idx] = spr;
      ++tmps_idx;
    }
  }

  ppu->tmp_sprite_len = tmps_idx;
}

static uint16_t nametable_addr(PPU *ppu) {
  uint16_t f = ppu->io.ctrl & 0x03;
  return 0x2000 + f * 0x400;
}

static uint16_t bg_paltable_addr(PPU *ppu) {
  uint16_t f = (ppu->io.ctrl >> 4) & 0x01;
  return f * 0x1000;
}

static uint16_t sprite_paltable_addr(PPU *ppu) {
  uint16_t f = (ppu->io.ctrl >> 3) & 0x01;
  return f * 0x1000;
}

void ppu_oam_write(PPU *ppu, uint8_t data) {
  // printf("%d\n", data);
  ppu->bus->oam[ppu->io.oamaddr++] = data;
}

static void ppu_draw_line(PPU *ppu, Disp screen) {
  ppu_fetch_sprite(ppu);
  uint8_t palette[4];

  /* draw background sprite */
  if(is_enable_sprite(ppu)) {
    for(uint8_t idx = 0; idx < ppu->tmp_sprite_len; ++idx) {
      Tile tile;
      Sprite sprite = ppu->tmp_sprite[idx];
      if(!(sprite.attr & (1 << 5))) continue;
      uint8_t pid = sprite.attr & 0x3;
      uint8_t vhflip = (sprite.attr >> 6) & 0x3;
      ppu_make_sprite_tile(ppu, &tile, sprite.tileid, pid, vhflip, sprite_paltable_addr(ppu));
      for(int i = 0; i < 4; ++i) {
        palette[i] = ppubus_read(ppu->bus, 0x3f10 + tile.paletteid * 4 + i);
      }

      for(int i = 0; i < 8; ++i) {
        uint8_t cidx = tile.pp[ppu->line - (sprite.y + 1)][i];
        if(cidx != 0) {
          RGB rgb = colors[palette[cidx]];
          // printf("@@%d %d\n", sprite.x + i, ppu->line);
          if(sprite.x + i > 255)
            break;
          put_pixel(screen, ppu->line, sprite.x + i, rgb);
        }
      }
    }
  }

  /* draw background */
  if(is_enable_bg(ppu)) {
    Tile tile;
    uint8_t y = ppu->line / 8;
    uint8_t y_in_tile = ppu->line % 8;
    for(uint8_t x = 0; x < 32; x++) {
      // printf("nametable %#x ", nametable_addr(ppu));
      // printf("scrollx: %d, scrolly: %d\n", ppu->scrollx / 8, ppu->scrolly / 8);
      ppu_make_bg_tile(ppu, &tile, x + ppu->scrollx / 8, y + ppu->scrolly / 8, nametable_addr(ppu), bg_paltable_addr(ppu));
      for(int i = 0; i < 4; ++i) {
        palette[i] = ppubus_read(ppu->bus, 0x3f00 + tile.paletteid * 4 + i);
      }

      for(int i = 0; i < 8; ++i) {
        uint8_t cidx = tile.pp[y_in_tile][i];
        if(cidx != 0) {
          RGB rgb = colors[palette[cidx]];
          put_pixel(screen, ppu->line, x * 8 + i, rgb);
        }
      }
    }
  }

  /* draw sprite */
  if(is_enable_sprite(ppu)) {
    for(uint8_t idx = 0; idx < ppu->tmp_sprite_len; ++idx) {
      Tile tile;
      Sprite sprite = ppu->tmp_sprite[idx];
      if(sprite.attr & (1 << 5)) continue;
      uint8_t pid = sprite.attr & 0x3;
      uint8_t vhflip = (sprite.attr >> 6) & 0x3;
      ppu_make_sprite_tile(ppu, &tile, sprite.tileid, pid, vhflip, sprite_paltable_addr(ppu));
      // tile_dump(&tile);
      for(int i = 0; i < 4; ++i) {
        palette[i] = ppubus_read(ppu->bus, 0x3f10 + tile.paletteid * 4 + i);
      }

      for(int i = 0; i < 8; ++i) {
        uint8_t cidx = tile.pp[ppu->line - (sprite.y + 1)][i];
        if(cidx != 0) {
          RGB rgb = colors[palette[cidx]];
          if(sprite.x + i > 255)
            break;
          put_pixel(screen, ppu->line, sprite.x + i, rgb);
        }
      }
    }
  }
}

int ppu_step(PPU *ppu, Disp screen, int *nmi) {
  switch(linestate_from(ppu->line)) {
    case VISIBLE:
      ppu_draw_line(ppu, screen);
      ppu->line++;
      break;
    case POSTRENDER:
      ppu->line++;
      break;
    case VERTICAL_BLANKING:
      if(ppu->line == 241) {
        enable_VBlank(ppu);
        *nmi = is_enable_nmi(ppu)? 1 : 0;
      }
      ppu->line++;
      break;
    case PRERENDER:
      ppu->line = 0;
      ppu->io.status = 0;
      disable_VBlank(ppu);
      return ppubus_read(ppu->bus, 0x3f00);
  }
  return 0;
}
