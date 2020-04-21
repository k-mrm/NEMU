#include "cpu/cpu.h"

CPU cpu;

void cpu_init() {
    cpu.a = 0;
    cpu.x = 0;
    cpu.y = 0;
    cpu.pc = 0;
    cpu.sp = 0xfd;
    cpu.p = 0x34;
}
