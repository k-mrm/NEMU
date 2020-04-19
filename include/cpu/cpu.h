#ifndef NEMU_CPU_H
#define NEMU_CPU_H

#include <stdint.h>

enum REGP_STATUS {
    P_STATUS_CARRY      = 0b00000001,
    P_STATUS_ZERO       = 0b00000010,
    P_STATUS_IRQ        = 0b00000100,
    P_STATUS_DECIMAL    = 0b00001000,
    P_STATUS_BRK        = 0b00010000,
    P_STATUS_RESERVED   = 0b00100000,   /* always 1 */
    P_STATUS_OVERFLOW   = 0b01000000,
    P_STATUS_NEGATIVE   = 0b10000000,
};

typedef struct CPU CPU;
struct CPU {
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint16_t pc;
    uint8_t sp;
    uint8_t p;
};

extern CPU cpu;

#endif
