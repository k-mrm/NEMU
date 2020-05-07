#include <stdlib.h>
#include "ppu/ppu.h"

uint8_t ppu_read(PPU *ppu, uint16_t idx) {
    switch(idx) {
    case 7: {
        uint8_t data = ppubus_read(ppu->bus, ppu->addr);
        printf("ppuaddr read %#x\n", ppu->addr);
        ppu->addr++;
        return data;
    }
    default:
        break;
    }
}

void ppu_write(PPU *ppu, uint16_t idx, uint8_t data) {
    switch(idx) {
    case 3:
        ppu->reg.oamaddr = data;
        break;
    case 4:
        ppu->bus->oam[ppu->reg.oamaddr] = data;
        ppu->reg.oamaddr++;
        break;
    case 6:
        ppu->addr = ppu->state.addr_write_once ?
            ppu->addr | data :
            (uint16_t)data << 8;
        ppu->state.addr_write_once ^= 1;
        break;
    case 7:
        ppubus_write(ppu->bus, ppu->addr, data);
        printf("ppuaddr write %#x\n", ppu->addr);
        ppu->addr++;
        break;
    default:
        break;
    }
}

void ppu_init(PPU *ppu, PPUBus *bus) {
    ppu->reg.ctrl = 0;
    ppu->reg.mask = 0;
    ppu->reg.status = 0;
    ppu->reg.oamaddr = 0;
    ppu->reg.scroll = 0;
    ppu->reg.addr = 0;
    ppu->reg.data = 0;
    ppu->reg.oamdma = 0;

    ppu->state.addr_write_once = false;

    ppu->addr = 0;
    ppu->bus = bus;
    ppu->cpu_cycle = 0;
}

void ppu_step(PPU *ppu, int cycle) {
    ppu->cpu_cycle += cycle;
    if(ppu->cpu_cycle >= 341) {
        ppu->cpu_cycle -= 341;
    }
}
