#include "cpu/cpu.h"

uint8_t cpubus[0x10000];

void cpu_init(CPU *cpu) {
    cpu->reg.a = 0;
    cpu->reg.x = 0;
    cpu->reg.y = 0;
    cpu->reg.pc = 0;
    cpu->reg.sp = 0xfd;
    cpu->reg.p = 0x34;
    cpu->bus = cpubus;
}
