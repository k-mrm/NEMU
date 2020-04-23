#ifndef NEMU_CPU_BUS_H
#define NEMU_CPU_BUS_H

#include <stdint.h>
#include "ram/ram.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "cassette/cassette.h"

typedef struct CPUBus CPUBus;
struct CPUBus {
    RAM *wram;
    PPU *ppu;
    APU *apu;
    Cassette *cas;
};

uint8_t cpubus_read(CPUBus *, int);
void cpubus_write(CPUBus *, int, uint8_t);

#endif
