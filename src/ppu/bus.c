#include <stdlib.h>
#include "ppu/bus.h"

uint8_t ppubus_read(PPUBus *bus, uint16_t addr) {
    if(addr < 0x2000) {
        return cassette_read_chrrom(bus->cassette, addr);
    }
    return bus->vram[addr - 0x2000];
}

void ppubus_write(PPUBus *bus, uint16_t addr, uint8_t data) {
    bus->vram[addr - 0x2000] = data;
}

void ppubus_init(PPUBus *bus, Cassette *cas) {
    bus->cassette = cas;
    bus->vram = malloc(sizeof(uint8_t) * 0x800); /* 2 KiB */
    bus->palette = malloc(sizeof(uint8_t) * 0x20);
}
