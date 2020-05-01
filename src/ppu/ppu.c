#include <stdlib.h>
#include "ppu/ppu.h"

uint8_t ppu_read(PPU *ppu, uint16_t addr) {
    return 0;
}

void ppu_write(PPU *ppu, uint16_t addr, uint8_t data) {
    ;
}

void ppu_init(PPU *ppu, Cassette *cas) {
    ppu->cassette = cas;
    ppu->vram = malloc(sizeof(uint8_t) * 8192);
}
