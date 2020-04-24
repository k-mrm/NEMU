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
        cpu_set_pc(cpu, ((uint16_t)high << 8) | low);
    }
    case IRQ:   break;
    case BRK:   break;
    default:    break; /* unreachable */
    }
}
