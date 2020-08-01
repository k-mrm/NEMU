#include <stdlib.h>
#include <string.h>
#include "ppu/ppu.h"
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

#define vramaddr_inc(ppu)   (is_addr_inc32((ppu))? 32 : 1)

#define is_enable_bg(ppu)     ((ppu)->io.mask & (1 << 3))
#define is_enable_sprite(ppu) ((ppu)->io.mask & (1 << 4))

#define fine_y(vramaddr) (((vramaddr) >> 12) & 0x7)

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
      if(ppu->vramaddr < 0x3f00) {
        res = ppu->data_buf;
        ppu->data_buf = data;
      }
      else {
        // printf("ppuaddr read %#x\n", ppu->vramaddr);
        res = data;
      }
      ppu->vramaddr += vramaddr_inc(ppu);
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
      ppu->vramaddr += vramaddr_inc(ppu);
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
  ppu->oam[ppu->io.oamaddr++] = data;
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
#define attr_addr(v)  (0x23c0 | ((v) & 0xc00) | (((v) >> 4) & 0x38) | (((v) >> 2) & 0x7))

static void bg_shift(PPU *ppu) {
  ppu->bglow_reg <<= 1;
  ppu->bghigh_reg <<= 1;
  ppu->attrlow_reg <<= 1;
  ppu->attrhigh_reg <<= 1;

  ppu->attrlow_reg |= (ppu->attr_latch & 0x1) != 0;
  ppu->attrhigh_reg |= (ppu->attr_latch & 0x2) != 0;
}

static void sprite_shift(PPU *ppu) {
  for(int i = 0; i < 8; i++) {
    if(ppu->snd_sprite_xcounter[i] > 0) {
      ppu->snd_sprite_xcounter[i]--;
    }
    else {
      ppu->snd_sprite_sprlow[i] <<= 1;
      ppu->snd_sprite_sprhigh[i] <<= 1;
    }
  }
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
      if(coarse_x(ppu->vramaddr) & 0x2) at >>= 2;
      if(coarse_y(ppu->vramaddr) & 0x2) at >>= 4;
      ppu->atbyte = at & 0x3;
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

static void draw_pixel(PPU *ppu, Disp screen) {
  uint8_t color = 0;

  uint8_t mux_mask = 0x80 >> ppu->fine_x;
  uint8_t lpixel = ((ppu->bglow_reg >> 8) & mux_mask) != 0;
  uint8_t hpixel = ((ppu->bghigh_reg >> 8) & mux_mask) != 0;
  uint8_t bgpixel = lpixel | (hpixel << 1);

  uint8_t lpid = (ppu->attrlow_reg & mux_mask) != 0;
  uint8_t hpid = (ppu->attrhigh_reg & mux_mask) != 0;
  uint8_t bgpid = lpid | (hpid << 1);

  uint8_t sprpixel = 0;
  uint8_t sprpid = 0;
  uint8_t priority = 0;
  if(is_enable_sprite(ppu)) {
    for(int i = 0; i < 8; i++) {
      if(ppu->snd_sprite_xcounter[i] == 0) {
        uint8_t splo = (ppu->snd_sprite_sprlow[i] & 0x80) != 0;
        uint8_t sphi = (ppu->snd_sprite_sprhigh[i] & 0x80) != 0;
        sprpixel = splo | (sphi << 1);
        sprpid = ppu->snd_sprite_atlatch[i] & 0x03;
        priority = (ppu->snd_sprite_atlatch[i] >> 5) & 0x01;

        if(i == 0 && ppu->sprite_0hit && sprpixel && bgpixel) {
          sprite_0hit(ppu);
          ppu->sprite_0hit = 0;
        }

        if(sprpixel) break;
      }
    }
  }

  if(bgpixel == 0 && sprpixel == 0) {
    color = ppubus_read(ppu->bus, 0x3f00);
  }
  else if(bgpixel == 0 && sprpixel) {
    color = ppubus_read(ppu->bus, 0x3f10 + (sprpid << 2) + sprpixel);
  }
  else if(bgpixel && sprpixel == 0) {
    color = ppubus_read(ppu->bus, 0x3f00 + (bgpid << 2) + bgpixel);
  }
  else if(bgpixel && sprpixel) {
    if(!priority)
      color = ppubus_read(ppu->bus, 0x3f10 + (sprpid << 2) + sprpixel);
    else
      color = ppubus_read(ppu->bus, 0x3f00 + (bgpid << 2) + bgpixel);
  }

  RGB rgb = colors[color];
  put_pixel(screen, ppu->line, ppu->cycle - 2, rgb);
}

static void clear_snd_oam(PPU *ppu) {
  memset(ppu->snd_sprite, 0xff, sizeof(Sprite) * 8);
}

static void evaluate_sprite(PPU *ppu) {
  uint8_t snd_idx = 0;
  for(int i = 0; i < 64; ++i) {
    uint8_t y = ppu->oam[i * 4];
    if(snd_idx >= 8) break;

    if(y <= ppu->line && ppu->line < y + 8) {
      if(i == 0) {
        ppu->pre_sprite_0hit = 1;
      }

      ppu->snd_sprite[snd_idx++] = (Sprite){
        .y = ppu->oam[i * 4],
        .tileid = ppu->oam[i * 4 + 1],
        .attr = ppu->oam[i * 4 + 2],
        .x = ppu->oam[i * 4 + 3],
      };
    }
  }
}

static uint8_t rev_bit(uint8_t x) {
  x = ((x & 0x55) << 1) | ((x >> 1) & 0x55);
  x = ((x & 0x33) << 2) | ((x >> 2) & 0x33);
  x = ((x & 0x0f) << 4) | ((x >> 4) & 0x0f);
  return x;
}

static void fetch_sprite(PPU *ppu) {
  for(int i = 0; i < 8; i++) {
    uint8_t id = ppu->snd_sprite[i].tileid;
    uint8_t attr = ppu->snd_sprite[i].attr;
    uint8_t x = ppu->snd_sprite[i].x;
    if(id == 0xff && attr == 0xff && x == 0xff) {
      /* dummy fetch */
      ppu->snd_sprite_sprlow[i] = 0;
      ppu->snd_sprite_sprhigh[i] = 0;
    }
    else {
      uint8_t yoffset = ppu->line - ppu->snd_sprite[i].y;
      uint8_t vhflip = (attr >> 6) & 0x3;
      uint8_t vflip = vhflip & 0x2;
      uint8_t hflip = vhflip & 0x1;
      uint8_t sprlow =
        ppubus_read(ppu->bus, sprite_paltable_addr(ppu) + id * 16 + (vflip? 7 - yoffset : yoffset));
      uint8_t sprhigh =
        ppubus_read(ppu->bus, sprite_paltable_addr(ppu) + id * 16 + 8 + (vflip? 7 - yoffset : yoffset));

      ppu->snd_sprite_sprlow[i] = hflip? rev_bit(sprlow) : sprlow;
      ppu->snd_sprite_sprhigh[i] = hflip? rev_bit(sprhigh) : sprhigh;
    }

    ppu->snd_sprite_atlatch[i] = attr;
    ppu->snd_sprite_xcounter[i] = x;
  }
}

static void next_scanline(PPU *ppu) {
  if(ppu->pre_sprite_0hit) {
    ppu->sprite_0hit = 1;
    ppu->pre_sprite_0hit = 0;
  }

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
        if(is_enable_bg(ppu) || is_enable_sprite(ppu)) {
          if(2 <= ppu->cycle && ppu->cycle <= 257) {
            draw_pixel(ppu, screen);
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
        }

        if(is_enable_sprite(ppu)) {
          if(2 <= ppu->cycle && ppu->cycle <= 257)
            sprite_shift(ppu);
          if(ppu->cycle == 1)
            clear_snd_oam(ppu);
          if(ppu->cycle == 65)
            evaluate_sprite(ppu);
          if(ppu->cycle == 257)
            fetch_sprite(ppu);
        }
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

        if(is_enable_bg(ppu) || is_enable_sprite(ppu)) {
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
        }

        if(is_enable_sprite(ppu)) {
          if(2 <= ppu->cycle && ppu->cycle <= 257)
            sprite_shift(ppu);
          if(ppu->cycle == 1)
            clear_snd_oam(ppu);
          if(ppu->cycle == 65)
            evaluate_sprite(ppu);
          if(ppu->cycle == 257)
            fetch_sprite(ppu);
        }
        break;
    }
  }
  return ret;
}
