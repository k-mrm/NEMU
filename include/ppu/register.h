#ifndef NEMU_PPUREG_H
#define NEMU_PPUREG_H

#include "ppu.h"

struct PPUreg {
  uint8_t ctrl;       /* $2000 */
  uint8_t mask;       /* $2001 */
  uint8_t status;     /* $2002 */
  uint8_t oamaddr;    /* $2003 */
  uint8_t oamdata;    /* $2004 */
  uint8_t scroll;     /* $2005 */
  uint8_t addr;       /* $2006 */
  uint8_t data;       /* $2007 */
  uint8_t oamdma;     /* $4014 */
};

#define enable_VBlank(ppu)  (ppu)->reg.status |= (1 << 7)
#define disable_VBlank(ppu) (ppu)->reg.status &= ~(1 << 7)

#endif
