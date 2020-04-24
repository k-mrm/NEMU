#ifndef NEMU_CPU_INTERRUPT_H
#define NEMU_CPU_INTERRUPT_H

#include "cpu/cpu.h"

enum Interrupt {
    NMI,
    RESET,
    IRQ,
    BRK,
};

void cpu_interrupt(CPU *, int);

#endif
