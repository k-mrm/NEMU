#ifndef NEMU_CPU_H
#define NEMU_CPU_H

#include <stdint.h>

enum REGP_STATUS {
    P_STATUS_CARRY      = 1 << 0,
    P_STATUS_ZERO       = 1 << 1,
    P_STATUS_IRQ        = 1 << 2,
    P_STATUS_DECIMAL    = 1 << 3,
    P_STATUS_BRK        = 1 << 4,
    P_STATUS_RESERVED   = 1 << 5,   /* always 1 */
    P_STATUS_OVERFLOW   = 1 << 6,
    P_STATUS_NEGATIVE   = 1 << 7,
};

typedef struct CPU CPU;
struct CPU {
    struct {
        uint8_t a;
        uint8_t x;
        uint8_t y;
        uint16_t pc;
        uint8_t sp;
        uint8_t p;
    } reg;
    uint8_t *bus;
};

void cpu_init(CPU *);

#endif
