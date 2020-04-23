#include "cpu/cpu.h"
#include "cpu/bus.h"

uint8_t cpubus_read(CPU *cpu, int addr) {
    if(addr < 0x2000) {
        return ram_read(cpu->bus->wram, addr & 0x7ff);
    }
    else if(addr < 0x4000) {
        return ppu_read(cpu->bus->ppu, (addr - 0x2000) & 0x7);
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
        return cassette_read_rom(cpu->bus->cas, addr - 0x8000);
    }
    else if(addr < 0x10000 && cpu->bus->cas->nprgrom_byte <= 0x4000) {
        /* mirror $8000-$bfff == $c0000-$ffff */
        return cassette_read_rom(cpu->bus->cas, addr - 0xc000);
    }
    else {
        return cassette_read_rom(cpu->bus->cas, addr - 0x8000);
    }
}
