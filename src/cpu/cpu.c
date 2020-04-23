#include "cpu/cpu.h"

void cpu_init(CPU *cpu) {
    cpu->reg.a = 0;
    cpu->reg.x = 0;
    cpu->reg.y = 0;
    cpu->reg.pc = 0;
    cpu->reg.sp = 0xfd;
    cpu->reg.p = 0x34;
}

