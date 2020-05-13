#ifndef NEMU_PPU_H
#define NEMU_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "ppu/bus.h"
#include "ppu/palette.h"

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
    } reg;
    struct {
        bool addr_write_once;
    } state;
    uint16_t addr;
    uint16_t line;
    PPUBus *bus;
    uint16_t scrollx;
    uint16_t scrolly;
    int cpu_cycle;
};

typedef uint8_t Disp[256][240];

void ppu_run(PPU *, int);
void ppu_render(PPU *, Disp);
int ppu_step(PPU *, int, Disp);
void ppu_init(PPU *, PPUBus *);
uint8_t ppu_read(PPU *, uint16_t);
void ppu_write(PPU *, uint16_t, uint8_t);

#endif
