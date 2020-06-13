#include <stdlib.h>
#include "ppu/bus.h"
#include "log/log.h"

uint8_t ppubus_read(PPUBus *bus, uint16_t addr) {
  log_dbg("ppubus_read %#x\n", addr);
  uint8_t res;
  if(addr < 0x2000) {
    res = cassette_read_chrrom(bus->cassette, addr);
  }
  else if(addr < 0x3000) {
    /* TODO: Mirror */
    res = bus->vram[addr - 0x2000];
  }
  else if(addr < 0x3f00) {
    /* TODO: Mirror */
    res = bus->vram[addr - 0x3000];
  }
  else if(addr < 0x4000) {
    addr = 0x3f00 | (addr & 0x1f);
    // printf("read!! %u@%x ", bus->palette[(addr-0x3f00)&0x1f], (addr-0x3f00)&0x1f);
    if(addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c) {
      addr -= 0x10;
    }
    res = bus->palette[(addr - 0x3f00) & 0x1f];
  }
  else {
    /* unreachable */
    res = 0;
  }
  return res;
}

void ppubus_write(PPUBus *bus, uint16_t addr, uint8_t data) {
  log_dbg("ppubus_write %#x <- %u\n", addr, data);
  if(addr < 0x2000) {
    ;
  }
  else if(addr < 0x3000) {
    /* TODO: BG Mirror */
    bus->vram[addr - 0x2000] = data;
  }
  else if(addr < 0x3f00) {
    /* TODO: BG Mirror */
    bus->vram[addr - 0x3000] = data;
  }
  else if(addr < 0x4000) {
    addr = 0x3f00 | (addr & 0x1f);
    if(addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c) {
      addr -= 0x10;
    }
    bus->palette[(addr - 0x3f00) & 0x1f] = data;
    // printf("write!! %u@%x ", bus->palette[(addr-0x3f00)&0x1f], (addr-0x3f00)&0x1f);
  }
  else {
    /* unreachable */
  }
}

void ppubus_init(PPUBus *bus, Cassette *cas) {
  bus->cassette = cas;
  bus->vram = calloc(1, sizeof(uint8_t) * 0x800); /* 2 KiB */
  bus->palette = calloc(1, sizeof(uint8_t) * 0x20);
  bus->oam = calloc(1, sizeof(uint8_t) * 256);
}
