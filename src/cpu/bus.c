#include "cpu/cpu.h"
#include "cpu/bus.h"

uint8_t cpubus_read(CPU *cpu, int addr) {
    if(addr < 0x0800) {
    }
    else if(addr < 0x2000) {
        ;
    }
    else if(addr < 0x2008) {
        ;
    }
    else if(addr < 0x4000) {
        ;
    }
    else if(addr < 0x4020) {
        ;
    }
    else if(addr < 0x6000) {
        ;
    }
    else if(addr < 0x8000) {
        ;
    }
    else {
        ;
    }
}
