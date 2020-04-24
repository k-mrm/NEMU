#ifndef NEMU_PPU_H
#define NEMU_PPU_H

#include <stdint.h>

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
};

uint8_t ppu_read(PPU *, int);
void ppu_write(PPU *, int, uint8_t);

#endif