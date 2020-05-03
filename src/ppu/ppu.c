#include <stdlib.h>
#include "ppu/ppu.h"

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
    switch(idx) {
    case 7: {
        uint8_t data = ppubus_read(ppu->bus, ppu->addr);
        ppu->addr++;
        return data;
    }
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
        ppu->addr++;
        break;
    default:
        break;
    }
}

void ppu_init(PPU *ppu, PPUBus *bus) {
    ppu->bus = bus;
}
