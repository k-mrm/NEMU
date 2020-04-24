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

void cpubus_init(CPUBus *, RAM *, PPU *, APU *, Cassette *);
uint8_t cpubus_read(CPUBus *, uint16_t);
void cpubus_write(CPUBus *, uint16_t, uint8_t);

#endif
