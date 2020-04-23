#include "cpu/cpu.h"
#include "cpu/bus.h"

uint8_t cpubus_read(CPUBus *bus, int addr) {
    if(addr < 0x2000) {
        return ram_read(bus->wram, addr & 0x7ff);
    }
    else if(addr < 0x4000) {
        return ppu_read(bus->ppu, (addr - 0x2000) & 0x7);
    }
    else if(addr < 0x4018) {
        return 0;   /* TODO */
    }
    else if(addr < 0x4020) {
        return 0;   /* Normally disabled */
    }
    else if(addr < 0x6000) {
        return 0;   /* Mapper 0 */
    }
    else if(addr < 0x8000) {
        return 0;   /* Mapper 0 */
    }
    else if(addr < 0xc000) {
        return cassette_read_rom(bus->cas, addr - 0x8000);
    }
    else if(addr < 0x10000 && bus->cas->nprgrom_byte <= 0x4000) {
        /* mirror $8000-$bfff == $c0000-$ffff */
        return cassette_read_rom(bus->cas, addr - 0xc000);
    }
    else {
        return cassette_read_rom(bus->cas, addr - 0x8000);
    }
}

void cpubus_write(CPUBus *bus, int addr, uint8_t data) {
    if(addr < 0x2000) {
        return ram_write(bus->wram, addr & 0x7ff, data);
    }
    else if(addr < 0x4000) {
        return ppu_write(bus->ppu, (addr - 0x2000) & 0x7, data);
    }
    else if(addr < 0x4018) {
        ;   /* TODO */
    }
    else if(addr < 0x4020) {
        ;   /* Normally disabled */
    }
    else if(addr < 0x6000) {
        ;   /* Mapper 0 */
    }
    else if(addr < 0x8000) {
        ;   /* Mapper 0 */
    }
}
