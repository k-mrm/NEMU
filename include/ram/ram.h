#ifndef NEMU_RAM_H
#define NEMU_RAM_H

#include <stdint.h>

typedef struct RAM RAM;
struct RAM {
  uint8_t ram[0x800];
};

static inline uint8_t ram_read(RAM *r, int addr) {
  return r->ram[addr];
}

static inline void ram_write(RAM *r, int addr, uint8_t data) {
  r->ram[addr] = data;
}

#endif
