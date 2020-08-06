#ifndef NEMU_CPU_INST_H
#define NEMU_CPU_INST_H

enum AddressingMode {
  ADDR_ACCUMULATOR,
  ADDR_IMMEDIATE,
  ADDR_IMPLIED,
  ADDR_RELATIVE,
  ADDR_ZEROPAGE,
  ADDR_ZEROPAGEX,
  ADDR_ZEROPAGEY,
  ADDR_ABSOLUTE,
  ADDR_ABSOLUTEX,
  ADDR_ABSOLUTEY,
  ADDR_INDIRECT,
  ADDR_INDIRECTX,
  ADDR_INDIRECTY,
};

enum opcode {
#define OP(op) OP_ ## op,
#include "cpu/opcode.h"
#undef OP
};

typedef struct CPUInst CPUInst;

struct CPUInst {
  enum opcode op;
  enum AddressingMode a;
  int len;
  int cycle;
};

extern CPUInst code_decoder[256];

#define DEF_INST(hex, op, a, len, cycle)  \
  code_decoder[(hex)] = (CPUInst){(op), (a), (len), (cycle)}

#define INVALID_INST(hex) \
  code_decoder[(hex)] = (CPUInst){255, 255, 0, 0}

void cpu_define_inst(void);
int cpu_step(CPU *);
void cpu_run(CPU *, int);

#endif
