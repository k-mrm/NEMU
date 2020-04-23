#ifndef NEMU_CPU_BUS_H
#define NEMU_CPU_BUS_H

#include <stdint.h>
#include "ppu/ppu.h"

typedef struct CPUBus CPUBus;
struct CPUBus {
    uint8_t wram[0x800];    /* 0x0000 ~ 0x07ff */
    PPU *ppu;
};

#endif
