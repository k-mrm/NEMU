#include <string.h>
#include "ppu/dma.h"

void ppu_dma_write(PPU *ppu, CPUBus *bus, uint16_t addr) {
  addr <<= 8;
  for(int i = 0; i < 256; i++) {
    ppu->bus->oam[i] = cpubus_read(bus, addr + i);
  }
}
