#include <string.h>
#include "ppu/dma.h"

void ppu_dma_write(PPU *ppu, CPUBus *bus, uint16_t addr) {
  addr <<= 8;
  for(int i = 0; i < 64; i++) {
    i <<= 2;
    ppu->bus->oam[i].y = cpubus_read(bus, addr + i);
    ppu->bus->oam[i].tileid = cpubus_read(bus, addr + i + 1);
    ppu->bus->oam[i].attr = cpubus_read(bus, addr + i + 2);
    ppu->bus->oam[i].x = cpubus_read(bus, addr + i + 3);
  }
}
