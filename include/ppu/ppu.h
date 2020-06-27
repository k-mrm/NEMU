#ifndef NEMU_PPU_H
#define NEMU_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ppu/bus.h"
#include "ppu/palette.h"
#include "ppu/sprite.h"

typedef ALLEGRO_VERTEX Disp[240][256];

typedef struct PPU PPU;
struct PPU {
  struct {
    uint8_t ctrl;       /* $2000 */
    uint8_t mask;       /* $2001 */
    uint8_t status;     /* $2002 */
    uint8_t oamaddr;    /* $2003 */
    uint8_t oamdata;    /* $2004 */
    uint8_t scroll;     /* $2005 */
    uint8_t addr;       /* $2006 */
    uint8_t data;       /* $2007 */
    uint8_t oamdma;     /* $4014 */
  } io;
  /*
   *  15bit vramaddr
   *  yyy NN YYYYY XXXXX
   *  ||| || ||||| +++++-- coarse X scroll
   *  ||| || +++++-------- coarse Y scroll
   *  ||| ++-------------- nametable select
   *  +++----------------- fine Y scroll
   *  see http://wiki.nesdev.com/w/index.php/PPU_scrolling#PPU_internal_registers
   */
  uint16_t vramaddr: 15;
  uint16_t tmp_vramaddr: 15;
  /*
   *  3bit fine X scroll
   */
  uint8_t fine_x: 3;
  uint16_t line;
  uint16_t scrollx;
  uint16_t scrolly;
  uint16_t cpu_cycle;
  bool write_once;
  Sprite snd_sprite[8];
  uint8_t snd_sprite_len;
  PPUBus *bus;
};

void ppu_oam_write(PPU *, uint8_t);
void ppu_render(PPU *, Disp);
int ppu_step(PPU *, Disp, int *);
void ppu_init(PPU *, PPUBus *);
uint8_t ppu_read(PPU *, uint16_t);
void ppu_write(PPU *, uint16_t, uint8_t);

#endif
