#include <stdlib.h>
#include "ppu/bus.h"

uint8_t ppubus_read(PPUBus *bus, uint16_t addr) {
  if(addr < 0x2000) {
    return cassette_read_chrrom(bus->cassette, addr);
  }
  else if(addr < 0x3000) {
    /* TODO: Mirror */
    return bus->vram[addr - 0x2000];
  }
  else if(addr < 0x3f00) {
    /* TODO: Mirror */
    return bus->vram[addr - 0x3000];
  }
  else if(addr < 0x4000) {
    addr = 0x3f00 | (addr & 0x1f);
    // printf("read!! %u@%x ", bus->palette[(addr-0x3f00)&0x1f], (addr-0x3f00)&0x1f);
    if(addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c) {
      addr -= 0x10;
    }
    return bus->palette[(addr - 0x3f00) & 0x1f];
  }
  else {
    /* unreachable */
    return 0;
  }
}

void ppubus_write(PPUBus *bus, uint16_t addr, uint8_t data) {
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
    ;
  }
}

void ppubus_init(PPUBus *bus, Cassette *cas) {
  bus->cassette = cas;
  bus->vram = calloc(1, sizeof(uint8_t) * 0x800); /* 2 KiB */
  bus->palette = calloc(1, sizeof(uint8_t) * 0x20);
  bus->oam = malloc(sizeof(uint8_t) * 256);
  for(int i = 0; i < 64; ++i) {
    /* 0x4 = 0b100 */
    bus->oam[i] = (Sprite){0, 0, 0x4, 0};
  }
}
