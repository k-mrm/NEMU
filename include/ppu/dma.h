#ifndef NEMU_PPU_DMA_H
#define NEMU_PPU_DMA_H

#include <stdint.h>
#include "cpu/bus.h"
#include "ppu/ppu.h"

void ppu_dma_write(PPU *, CPUBus *, uint16_t);

#endif
