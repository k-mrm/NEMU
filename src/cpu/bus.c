#include "cpu/cpu.h"
#include "cpu/bus.h"

uint8_t cpubus_read(CPU *cpu, int addr) {
    if(addr < 0x2000) {
        return ram_read(cpu->bus->wram, addr & 0x7ff);
    }
    else if(addr < 0x4000) {
        return ppu_read(cpu->bus->ppu, (addr - 0x2000) & 0x7);
    }
    else if(addr < 0x4020) {
        ;
    }
    else if(addr < 0x6000) {
        ;
    }
    else if(addr < 0x8000) {
    }
    else {
        ;
    }
}
