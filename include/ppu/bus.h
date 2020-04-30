#ifndef NEMU_PPU_BUS_H
#define NEMU_PPU_BUS_H

#include "cassette/cassette.h"

typedef struct PPUBus PPUBus;
struct PPUBus {
    Cassette *cassette;
};

#endif
