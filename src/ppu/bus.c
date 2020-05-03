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
