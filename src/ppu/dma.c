#include <string.h>
#include "ppu/ppu.h"
#include "ppu/dma.h"
#include "log/log.h"

void ppu_dma_write(PPU *ppu, CPUBus *bus, uint16_t addr) {
  for(int i = 0; i < 256; i++) {
    log_dbg("oamwrite %d ", i);
    ppu_oam_write(ppu, cpubus_read(bus, addr * 0x100 + i));
  }
}
