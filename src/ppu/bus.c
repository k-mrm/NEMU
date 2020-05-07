#include <stdlib.h>
#include "ppu/bus.h"

uint8_t ppubus_read(PPUBus *bus, uint16_t addr) {
    if(addr < 0x2000) {
        return cassette_read_chrrom(bus->cassette, addr);
    }
    else if(addr < 0x3000) {
        /* TODO: Mirror */
        return bus->vram[addr - 0x2000];
    }
    else if(addr < 0x3f00) {
        /* TODO: Mirror */
        return bus->vram[addr - 0x3000];
    }
    else if(addr < 0x4000) {
        return bus->palette[(addr - 0x3f00) & 0x20];
    }
    else {
        /* unreachable */
        return 0;
    }
}

void ppubus_write(PPUBus *bus, uint16_t addr, uint8_t data) {
    if(addr < 0x2000) {
        ;
    }
    else if(addr < 0x3000) {
        /* TODO: Mirror */
        bus->vram[addr - 0x2000] = data;
    }
    else if(addr < 0x3f00) {
        /* TODO: Mirror */
        bus->vram[addr - 0x3000] = data;
    }
    else if(addr < 0x4000) {
        bus->palette[(addr - 0x3f00) & 0x20] = data;
    }
    else {
        /* unreachable */
        ;
    }
}

void ppubus_init(PPUBus *bus, Cassette *cas) {
    bus->cassette = cas;
    bus->vram = malloc(sizeof(uint8_t) * 0x800); /* 2 KiB */
    bus->palette = malloc(sizeof(uint8_t) * 0x20);
    bus->oam = malloc(sizeof(uint8_t) * 256);
}
