#include <stdlib.h>
#include "ppu/bus.h"
#include "log/log.h"

uint16_t vram_address(PPUBus *bus, uint16_t addr) {
  enum mirroring mirr = nes_mirroring(bus->cassette);
  // printf("%s\n", mirr? "VERTICAL" : "HORIZONTAL");
  switch(mirr) {
    case HORIZONTAL:
      /*
       *  A 0x2000~ == 0x2400~
       *  B 0x2800~ == 0x2c00~ 
       *  A A
       *  B B
       */
      if(addr < 0x2400) {
        return addr - 0x2000;
      }
      else if(addr < 0x2800) {
        return addr - 0x2400;
      }
      else if(addr < 0x2c00) {
        return addr - 0x2400;
      }
      else if(addr < 0x3000) {
        return addr - 0x2800;
      }
      break;
    case VERTICAL:
      /*
       *  A 0x2000~ == 0x2800~
       *  B 0x2400~ == 0x2c00~
       *  A B
       *  A B
       */
      if(addr < 0x2400) {
        return addr - 0x2000;
      }
      else if(addr < 0x2800) {
        return addr - 0x2000;
      }
      else if(addr < 0x2c00) {
        return addr - 0x2800;
      }
      else if(addr < 0x3000) {
        return addr - 0x2800;
      }
      break;
  }

  panic("invalid address");
  return 0;
}

uint16_t safe_vram_address(PPUBus *bus, uint16_t addr) {
  uint16_t res = vram_address(bus, addr);
  if(res >= 0x800) {
    panic("vram_address");
  }

  return res;
}


uint8_t ppubus_read(PPUBus *bus, uint16_t addr) {
  addr &= 0x3fff;
  // printf("ppubus_read %#x\n", addr);
  uint8_t res;
  if(addr < 0x2000) {
    res = cassette_read_chrrom(bus->cassette, addr);
  }
  else if(addr < 0x3000) {
    res = bus->vram[safe_vram_address(bus, addr)];
  }
  else if(addr < 0x3f00) {
    res = bus->vram[safe_vram_address(bus, addr - 0x1000)];
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
  addr &= 0x3fff;
  // log_dbg("ppubus_write %#x <- %u\n", addr, data);
  if(addr < 0x2000) {
    ;
  }
  else if(addr < 0x3000) {
    bus->vram[safe_vram_address(bus, addr)] = data;
  }
  else if(addr < 0x3f00) {
    bus->vram[safe_vram_address(bus, addr - 0x1000)] = data;
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
