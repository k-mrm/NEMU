#include "cpu/cpu.h"
#include "cpu/register.h"
#include "cpu/interrupt.h"

void cpu_interrupt(CPU *cpu, int inter) {
    switch(inter) {
    case NMI:   break;
    case RESET: {
        cpu_set_pflag(cpu, P_STATUS_IRQ);
        uint8_t low = cpubus_read(cpu->bus, 0xfffc);
        uint8_t high = cpubus_read(cpu->bus, 0xfffd);
        cpu->reg.pc = ((uint16_t)high << 8) | low;
    }
    case IRQ:   break;
    case BRK: {
        if(cpu_get_pflag(cpu, P_STATUS_IRQ)) break;

        cpu_write_pflag(cpu, P_STATUS_BRK, 1);
        cpu->reg.pc++;
        cpu_stack_push(cpu, cpu->reg.pc >> 8);
        cpu_stack_push(cpu, cpu->reg.pc & 0xff);
        cpu_stack_push(cpu, cpu->reg.p);

        cpu_write_pflag(cpu, P_STATUS_IRQ, 1);

        uint8_t low = cpubus_read(cpu->bus, 0xfffe);
        uint8_t high = cpubus_read(cpu->bus, 0xffff);

        cpu->reg.pc = ((uint16_t)high << 8) | low;

        break;
    }
    default:    break; /* unreachable */
    }
}
