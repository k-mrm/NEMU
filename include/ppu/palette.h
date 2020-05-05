#ifndef NEMU_PPU_PALETTE_H
#define NEMU_PPU_PALETTE_H

#include <stdint.h>

typedef struct RGB RGB; 
struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

extern RGB colors[];

#endif
