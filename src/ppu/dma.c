#include <string.h>
#include "ppu/dma.h"

void ppu_dma_write(PPU *ppu, CPUBus *bus, uint16_t addr) {
  addr *= 0x100;
  for(int i = 0; i < 64; i++) {
    printf("oamwrite %d\n", i);
    ppu->bus->oam[i].y = cpubus_read(bus, addr + i * 4);
    ppu->bus->oam[i].tileid = cpubus_read(bus, addr + i * 4 + 1);
    ppu->bus->oam[i].attr = cpubus_read(bus, addr + i * 4 + 2);
    ppu->bus->oam[i].x = cpubus_read(bus, addr + i * 4 + 3);
  }
}
