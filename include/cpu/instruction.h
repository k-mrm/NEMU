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
  OP_ADC,
  OP_SBC,
  OP_AND,
  OP_ORA,
  OP_EOR,
  OP_ASL,
  OP_LSR,
  OP_ROL,
  OP_ROR,
  OP_BCC,
  OP_BCS,
  OP_BEQ,
  OP_BNE,
  OP_BVC,
  OP_BVS,
  OP_BPL,
  OP_BMI,
  OP_BIT,
  OP_JMP,
  OP_JSR,
  OP_RTS,
  OP_BRK,
  OP_RTI,
  OP_CMP,
  OP_CPX,
  OP_CPY,
  OP_INC,
  OP_DEC,
  OP_INX,
  OP_DEX,
  OP_INY,
  OP_DEY,
  OP_CLC,
  OP_SEC,
  OP_CLI,
  OP_SEI,
  OP_CLD,
  OP_SED,
  OP_CLV,
  OP_LDA,
  OP_LDX,
  OP_LDY,
  OP_STA,
  OP_STX,
  OP_STY,
  OP_TAX,
  OP_TXA,
  OP_TAY,
  OP_TYA,
  OP_TSX,
  OP_TXS,
  OP_PHA,
  OP_PLA,
  OP_PHP,
  OP_PLP,
  OP_NOP,
  /* unofficial opcode */
  OP_AAC,
  OP_AAX,
  OP_ARR,
  OP_ASR,
  OP_ATX,
  OP_AXA,
  OP_AXS,
  OP_DCP,
  OP_DOP,
  OP_ISC,
  OP_KIL,
  OP_LAR,
  OP_LAX,
  OP_RLA,
  OP_RRA,
  OP_SLO,
  OP_SRE,
  OP_SXA,
  OP_SYA,
  OP_TOP,
  OP_XAA,
  OP_XAS,
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
