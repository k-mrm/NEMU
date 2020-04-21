#include "cpu/cpu.h"
#include "cpu/interrupt.h"

void cpu_interrupt(int inter) {
    switch(inter) {
    case NMI:
    case RESET:
    case IRQ:
    case BRK:
    default: break; /* unreachable */
    }
}
