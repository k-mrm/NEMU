#include "cpu/cpu.h"
#include "cpu/register.h"

void cpu_init(CPU *cpu, CPUBus *cpubus) {
  cpu->reg.a = 0;
  cpu->reg.x = 0;
  cpu->reg.y = 0;
  cpu->reg.pc = 0;
  cpu->reg.sp = 0xfd;
  /*
   * reg.p:
   *    NV*BDIZC
   *    00110100
   */
  cpu->reg.p = 0x24; // 0x34?
  cpu->bus = cpubus;
}

void cpu_stack_push(CPU *cpu, uint8_t data) {
  cpubus_write(cpu->bus, cpu_stackptr(cpu), data);
  cpu->reg.sp--;
}

uint8_t cpu_stack_pop(CPU *cpu) {
  cpu->reg.sp++;
  return cpubus_read(cpu->bus, cpu_stackptr(cpu));
}

