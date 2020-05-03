#include <stdlib.h>
#include "ppu/ppu.h"

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
    switch(idx) {
    case 7:
        return ppubus_read(ppu->bus, ppu->addr);
    default:
        break;
    }
}

void ppu_write(PPU *ppu, uint16_t idx, uint8_t data) {
    switch(idx) {
    case 6:
        ppu->addr = ppu->state.addr_write_once ?
            ppu->addr | data :
            (uint16_t)data << 8;
        ppu->state.addr_write_once ^= 1;
        break;
    case 7:
        ppubus_write(ppu->bus, ppu->addr, data);
        break;
    default:
        break;
    }
}

void ppu_init(PPU *ppu, Cassette *cas) {
    ppu->bus->cassette = cas;
    ppu->bus->vram = malloc(sizeof(uint8_t) * 8192);
}
