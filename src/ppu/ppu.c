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

#define fine_y(vramaddr) (((vramaddr) >> 12) & 0x7)

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
      // printf("ppuaddr read %#x\n", ppu->vramaddr);
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
    case 0:
      /*
       *  t: ...BA.. ........ = d: ......BA
       */
      ppu->io.ctrl = data;
      ppu->tmp_vramaddr =
        (ppu->tmp_vramaddr & 0b111001111111111) | ((data & 0x3) << 10);
      break;
    case 1: ppu->io.mask = data; break;
    case 3: ppu->io.oamaddr = data; break;
    case 4: ppu_oam_write(ppu, data); break;
    case 5:
      if(!ppu->write_once) {
        /*
         *  t: ....... ...HGFED = d: HGFED...
         *  x:              CBA = d: .....CBA
         */
        ppu->fine_x = data & 0x7;
        uint8_t x_scroll = data >> 3;
        ppu->tmp_vramaddr =
          (ppu->tmp_vramaddr & 0b111111111100000) | x_scroll;
      }
      else {
        /* t: CBA..HG FED..... = d: HGFEDCBA */
        uint16_t fine_y = data & 0x7;
        uint16_t y_scroll = data >> 3;
        ppu->tmp_vramaddr =
          (ppu->tmp_vramaddr & 0b000110000011111) | (y_scroll << 5) | (fine_y << 12);
      }
      ppu->write_once ^= 1;
      break;
    case 6:
      if(!ppu->write_once) {
        /*
         *  t: .FEDCBA ........ = d: ..FEDCBA
         *  t: X...... ........ = 0
         */
        uint16_t high_addr = data & 0x3f;
        ppu->tmp_vramaddr =
          (ppu->tmp_vramaddr & 0b000000011111111) | (high_addr << 8);
      }
      else {
        /*
         *  t: ....... HGFEDCBA = d: HGFEDCBA
         *  v                   = t
         */
        ppu->tmp_vramaddr =
          (ppu->tmp_vramaddr & 0b111111100000000) | data;
        ppu->vramaddr = ppu->tmp_vramaddr;
      }
      ppu->write_once ^= 1;
      break;
    case 7:
      ppubus_write(ppu->bus, ppu->vramaddr, data);
      ppu->vramaddr += ppu_vramaddr_inc(ppu);
      break;
    default:
      break;
  }
}

void ppu_init(PPU *ppu, PPUBus *bus) {
  memset(ppu, 0, sizeof(PPU));
  ppu->bus = bus;
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
  ppu->snd_sprite_len = 0;

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
      if(i == 0)
        sprite_0hit(ppu);
      ppu->snd_sprite[tmps_idx] = spr;
      ++tmps_idx;
    }
  }

  ppu->snd_sprite_len = tmps_idx;
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

/*
 *  see http://wiki.nesdev.com/w/index.php/PPU_scrolling#Wrapping_around
 */
static void hori_increment(PPU *ppu) {
  if((ppu->vramaddr & 0x1f) == 0x1f) {
    ppu->vramaddr &= ~0x1f;
    ppu->vramaddr ^= 0x400;
  }
  else {
    ppu->vramaddr++;
  }
}

static void vert_increment(PPU *ppu) {
  if((ppu->vramaddr & 0x7000) == 0x7000) {
    ppu->vramaddr &= ~0x7000;
    uint8_t coarse_y = (ppu->vramaddr >> 5) & 0x1f;
    if(coarse_y == 29) {
      ppu->vramaddr &= ~0x3e0;
      ppu->vramaddr ^= 0x800;
    }
    else if(coarse_y == 31) {
      ppu->vramaddr &= ~0x3e0;
    }
    else {
      ppu->vramaddr = (ppu->vramaddr & ~0x3e0) | ((coarse_y + 1) << 5);
    }
  }
  else {
    ppu->vramaddr += 0x1000;
  }
}

static void copy_horizontal_t2v(PPU *ppu) {
  /*
   *  v: ....F.. ...EDCBA = t: ....F.. ...EDCBA
   */
  ppu->vramaddr =
    (ppu->vramaddr & ~0x41f) | (ppu->tmp_vramaddr & 0x41f);
}

static void copy_vertical_t2v(PPU *ppu) {
  /*
   *  v: IHGF.ED CBA..... = t: IHGF.ED CBA.....
   */
  ppu->vramaddr =
    (ppu->vramaddr & ~0x7be0) | (ppu->tmp_vramaddr & 0x7be0);
}

#define tile_addr(v)  (0x2000 | ((v) & 0xfff))
#define attr_addr(v)  (0x23c0 | ((v) & 0xc00) | (((v) >> 5) & 0x38) | (((v) >> 2) & 0x7))

static void bg_shift(PPU *ppu) {
  ppu->bglow_reg <<= 1;
  ppu->bghigh_reg <<= 1;
  ppu->attrlow_reg <<= 1;
  ppu->attrhigh_reg <<= 1;

  ppu->attrlow_reg |= (ppu->attr_latch & 0x1) != 0;
  ppu->attrhigh_reg |= (ppu->attr_latch & 0x2) != 0;
}

#define coarse_x(vaddr) ((vaddr) & 0x1f)
#define coarse_y(vaddr) (((vaddr) >> 5) & 0x1f)

static void update_background(PPU *ppu) {
  switch(ppu->cycle % 8) {
    case 1:
      ppu->ntbyte = ppubus_read(ppu->bus, tile_addr(ppu->vramaddr));
      break;
    case 3: {
      uint8_t at = ppubus_read(ppu->bus, attr_addr(ppu->vramaddr));
      uint8_t blockpos = coarse_x(ppu->vramaddr) % 4 / 2 + coarse_y(ppu->vramaddr) % 4 / 2 * 2;
      ppu->atbyte = (at >> blockpos) & 0x03;
      break;
    }
    case 5:
      ppu->lowtile = ppubus_read(ppu->bus, bg_paltable_addr(ppu) + ppu->ntbyte * 16 + fine_y(ppu->vramaddr));
      break;
    case 7:
      ppu->hightile = ppubus_read(ppu->bus, bg_paltable_addr(ppu) + ppu->ntbyte * 16 + 8 + fine_y(ppu->vramaddr));
      break;
    default: return;
  }
}

static void reload_shifter(PPU *ppu) {
  ppu->bglow_reg = (ppu->bglow_reg & 0xff00) | ppu->lowtile;
  ppu->bghigh_reg = (ppu->bghigh_reg & 0xff00) | ppu->hightile;
  ppu->attr_latch = ppu->atbyte;
}

static void draw_bgpixel(PPU *ppu, Disp screen) {
  uint8_t mux_mask = 0x80 >> ppu->fine_x;
  uint8_t lpixel = ((ppu->bglow_reg >> 8) & mux_mask) != 0;
  uint8_t hpixel = ((ppu->bghigh_reg >> 8) & mux_mask) != 0;
  uint8_t pixel = lpixel | (hpixel << 1);
  uint8_t lpid = (ppu->attrlow_reg & mux_mask) != 0;
  uint8_t hpid = (ppu->attrhigh_reg & mux_mask) != 0;
  uint8_t pid = lpid | (hpid << 1);

  uint8_t color = ppubus_read(ppu->bus, 0x3f00 + pid * 4 + pixel);
  // printf("pid %d pixel %d %#x color %d\n", pid, pixel, 0x3f00 + pid * 4 + pixel, color);
  RGB rgb = colors[color];

  put_pixel(screen, ppu->line, ppu->cycle - 2, rgb);
}

static void ppu_draw_line(PPU *ppu, Disp screen) {
  ppu_fetch_sprite(ppu);
  uint8_t palette[4];

  /* draw background sprite */
  if(is_enable_sprite(ppu)) {
    for(uint8_t idx = 0; idx < ppu->snd_sprite_len; ++idx) {
      Tile tile;
      Sprite sprite = ppu->snd_sprite[idx];
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

  /* draw sprite */
  if(is_enable_sprite(ppu)) {
    for(uint8_t idx = 0; idx < ppu->snd_sprite_len; ++idx) {
      Tile tile;
      Sprite sprite = ppu->snd_sprite[idx];
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

static void next_scanline(PPU *ppu) {
  if(linestate_from(ppu->line) == PRERENDER)
    ppu->line = 0;
  else
    ppu->line++;
}

static void ppu_tick(PPU *ppu) {
  if(ppu->cycle == 340) {
    ppu->cycle = 0;
    next_scanline(ppu);
  }
  else {
    ppu->cycle++;
  }
}

int ppu_step(PPU *ppu, Disp screen, int *nmi, int ncycle) {
  int ret = 0;
  while(ncycle--) {
    ppu_tick(ppu);

    /* see https://wiki.nesdev.com/w/images/4/4f/Ppu.svg */
    switch(linestate_from(ppu->line)) {
      case VISIBLE:
        if(2 <= ppu->cycle && ppu->cycle <= 257) {
          draw_bgpixel(ppu, screen);
        }
        if((1 <= ppu->cycle && ppu->cycle <= 256) || (321 <= ppu->cycle && ppu->cycle <= 336)) {
          update_background(ppu);
          if(ppu->cycle % 8 == 0)
            hori_increment(ppu);
        }
        if((2 <= ppu->cycle && ppu->cycle <= 257) || (322 <= ppu->cycle && ppu->cycle <= 337)) {
          bg_shift(ppu);
          if((ppu->cycle - 1) % 8 == 0)
            reload_shifter(ppu);
        }
        if(ppu->cycle == 256)
          vert_increment(ppu);
        if(ppu->cycle == 257)
          copy_horizontal_t2v(ppu);
        break;
      case POSTRENDER:
        break;
      case VERTICAL_BLANKING:
        if(ppu->line == 241 && ppu->cycle == 1) {
          enable_VBlank(ppu);
          *nmi = is_enable_nmi(ppu)? 1 : 0;
        }
        break;
      case PRERENDER:
        if(ppu->cycle == 1) {
          ppu->io.status = 0;
          disable_VBlank(ppu);
        }
        if((1 <= ppu->cycle && ppu->cycle <= 256) || (321 <= ppu->cycle && ppu->cycle <= 336)) {
          update_background(ppu);
          if(ppu->cycle % 8 == 0)
            hori_increment(ppu);
        }
        if((2 <= ppu->cycle && ppu->cycle <= 257) || (322 <= ppu->cycle && ppu->cycle <= 337)) {
          bg_shift(ppu);
          if((ppu->cycle - 1) % 8 == 0)
            reload_shifter(ppu);
        }
        if(ppu->cycle == 256)
          vert_increment(ppu);
        if(ppu->cycle == 257)
          copy_horizontal_t2v(ppu);
        if(280 <= ppu->cycle && ppu->cycle <= 304)
          copy_vertical_t2v(ppu);
        ret = ppubus_read(ppu->bus, 0x3f00);
        break;
    }
  }
  return ret;
}
