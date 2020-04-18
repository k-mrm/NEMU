#ifndef NEMU_CPU_H
#define NEMU_CPU_H

typedef struct CPU CPU;

enum {
    P_STATUS_CARRY      = 0b00000001,
    P_STATUS_ZERO       = 0b00000010,
    P_STATUS_IRQ        = 0b00000100,
    P_STATUS_DECIMAL    = 0b00001000,
    P_STATUS_BRK        = 0b00010000,
    P_STATUS_RESERVED   = 0b00100000,
    P_STATUS_OVERFLOW   = 0b01000000,
    P_STATUS_NEGATIVE   = 0b10000000,
};

struct CPU {
    unsigned char a;
    unsigned char x;
    unsigned char y;
    unsigned short pc;
    unsigned char sp;
    unsigned char p;
};

extern CPU cpu;

#endif
