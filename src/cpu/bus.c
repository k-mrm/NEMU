#include "cpu/cpu.h"
#include "cpu/bus.h"
#include "ppu/dma.h"
#include "log/log.h"

void cpubus_init(CPUBus *bus, RAM *r, PPU *p, APU *a, Cassette *c, Joypad *pad) {
  bus->wram = r;
  bus->ppu = p;
  bus->apu = a;
  bus->cas = c;
  bus->pad = pad;
}

uint8_t cpubus_read(CPUBus *bus, uint16_t addr) {
  // log_dbg("cpubus_read %#x\n", addr);
  if(addr < 0x2000) {
    return ram_read(bus->wram, addr & 0x7ff);
  }
  else if(addr < 0x4000) {
    return ppu_read(bus->ppu, (addr - 0x2000) & 0x7);
  }
  else if(addr == 0x4016) {
    return joypad_read(bus->pad, 1);
  }
  else if(addr == 0x4017) {
    return joypad_read(bus->pad, 2);
  }
  else if(addr < 0x4018) {
    return 0;   /* TODO */
  }
  else if(addr < 0x4020) {
    return 0;   /* Normally disabled */
  }
  else if(addr < 0x6000) {
    return 0;   /* Mapper 0 */
  }
  else if(addr < 0x8000) {
    return 0;   /* Mapper 0 */
  }
  else if(addr < 0xc000) {
    return cassette_read_rom(bus->cas, addr - 0x8000);
  }
  else if(bus->cas->nprgrom_byte <= 0x4000) {
    /* mirror $8000-$bfff == $c0000-$ffff */
    return cassette_read_rom(bus->cas, addr - 0xc000);
  }
  else {
    return cassette_read_rom(bus->cas, addr - 0x8000);
  }
}

void cpubus_write(CPUBus *bus, uint16_t addr, uint8_t data) {
  log_dbg("cpubus_write %#x <- %u\n", addr, data);
  if(addr < 0x2000) {
    ram_write(bus->wram, addr & 0x7ff, data);
  }
  else if(addr < 0x4000) {
    ppu_write(bus->ppu, (addr - 0x2000) & 0x7, data);
  }
  else if(addr == 0x4014) {
    ppu_dma_write(bus->ppu, bus, data);
  }
  else if(addr == 0x4016) {
    joypad_write(bus->pad, 1, data);
  }
  else if(addr == 0x4017) {
  }
  else if(addr < 0x4018) {
    ;   /* TODO */
  }
  else if(addr < 0x4020) {
    ;   /* Normally disabled */
  }
  else if(addr < 0x6000) {
    ;   /* Mapper 0 */
  }
  else if(addr < 0x8000) {
    ;   /* Mapper 0 */
  }
}
