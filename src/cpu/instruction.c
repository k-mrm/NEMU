#include "cpu/cpu.h"
#include "cpu/instruction.h"
#include "cpu/interrupt.h"
#include "cpu/register.h"
#include "log/log.h"

CPUInst code_decoder[256];

static char *inst_dump(uint8_t);

void cpu_define_inst() {
  DEF_INST(0x69, OP_ADC, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x65, OP_ADC, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x75, OP_ADC, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x6D, OP_ADC, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x7D, OP_ADC, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x79, OP_ADC, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0x61, OP_ADC, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0x71, OP_ADC, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0xE9, OP_SBC, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xE5, OP_SBC, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xF5, OP_SBC, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0xED, OP_SBC, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0xFD, OP_SBC, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0xF9, OP_SBC, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0xE1, OP_SBC, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0xF1, OP_SBC, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0x29, OP_AND, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x25, OP_AND, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x35, OP_AND, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x2D, OP_AND, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x3D, OP_AND, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x39, OP_AND, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0x21, OP_AND, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0x31, OP_AND, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0x09, OP_ORA, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x05, OP_ORA, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x15, OP_ORA, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x0D, OP_ORA, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x1D, OP_ORA, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x19, OP_ORA, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0x01, OP_ORA, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0x11, OP_ORA, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0x49, OP_EOR, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x45, OP_EOR, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x55, OP_EOR, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x4D, OP_EOR, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x5D, OP_EOR, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x59, OP_EOR, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0x41, OP_EOR, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0x51, OP_EOR, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0x0A, OP_ASL, ADDR_ACCUMULATOR, 1, 2);
  DEF_INST(0x06, OP_ASL, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x16, OP_ASL, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x0E, OP_ASL, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x1E, OP_ASL, ADDR_ABSOLUTEX, 3, 6);

  DEF_INST(0x4A, OP_LSR, ADDR_ACCUMULATOR, 1, 2);
  DEF_INST(0x46, OP_LSR, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x56, OP_LSR, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x4E, OP_LSR, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x5E, OP_LSR, ADDR_ABSOLUTEX, 3, 6);

  DEF_INST(0x2A, OP_ROL, ADDR_ACCUMULATOR, 1, 2);
  DEF_INST(0x26, OP_ROL, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x36, OP_ROL, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x2E, OP_ROL, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x3E, OP_ROL, ADDR_ABSOLUTEX, 3, 6);

  DEF_INST(0x6A, OP_ROR, ADDR_ACCUMULATOR, 1, 2);
  DEF_INST(0x66, OP_ROR, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x76, OP_ROR, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x6E, OP_ROR, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x7E, OP_ROR, ADDR_ABSOLUTEX, 3, 6);

  DEF_INST(0x90, OP_BCC, ADDR_RELATIVE, 2, 2);
  DEF_INST(0xB0, OP_BCS, ADDR_RELATIVE, 2, 2);
  DEF_INST(0xF0, OP_BEQ, ADDR_RELATIVE, 2, 2);
  DEF_INST(0xD0, OP_BNE, ADDR_RELATIVE, 2, 2);
  DEF_INST(0x50, OP_BVC, ADDR_RELATIVE, 2, 2);
  DEF_INST(0x70, OP_BVS, ADDR_RELATIVE, 2, 2);
  DEF_INST(0x10, OP_BPL, ADDR_RELATIVE, 2, 2);
  DEF_INST(0x30, OP_BMI, ADDR_RELATIVE, 2, 2);

  DEF_INST(0x24, OP_BIT, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x2C, OP_BIT, ADDR_ABSOLUTE, 3, 4);

  DEF_INST(0x4C, OP_JMP, ADDR_ABSOLUTE, 3, 3);
  DEF_INST(0x6C, OP_JMP, ADDR_INDIRECT, 3, 5);

  DEF_INST(0x20, OP_JSR, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x60, OP_RTS, ADDR_IMPLIED, 1, 6);
  DEF_INST(0x00, OP_BRK, ADDR_IMPLIED, 1, 7);
  DEF_INST(0x40, OP_RTI, ADDR_IMPLIED, 1, 6);

  DEF_INST(0xC9, OP_CMP, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xC5, OP_CMP, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xD5, OP_CMP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0xCD, OP_CMP, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0xDD, OP_CMP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0xD9, OP_CMP, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0xC1, OP_CMP, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0xD1, OP_CMP, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0xE0, OP_CPX, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xE4, OP_CPX, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xEC, OP_CPX, ADDR_ABSOLUTE, 3, 4);

  DEF_INST(0xC0, OP_CPY, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xC4, OP_CPY, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xCC, OP_CPY, ADDR_ABSOLUTE, 3, 4);

  DEF_INST(0xE6, OP_INC, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0xF6, OP_INC, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0xEE, OP_INC, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0xFE, OP_INC, ADDR_ABSOLUTEX, 3, 6);

  DEF_INST(0xC6, OP_DEC, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0xD6, OP_DEC, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0xCE, OP_DEC, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0xDE, OP_DEC, ADDR_ABSOLUTEX, 3, 6);

  DEF_INST(0xE8, OP_INX, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xCA, OP_DEX, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xC8, OP_INY, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x88, OP_DEY, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x18, OP_CLC, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x38, OP_SEC, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x58, OP_CLI, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x78, OP_SEI, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xD8, OP_CLD, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xF8, OP_SED, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xB8, OP_CLV, ADDR_IMPLIED, 1, 2);

  DEF_INST(0xA9, OP_LDA, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xA5, OP_LDA, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xB5, OP_LDA, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0xAD, OP_LDA, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0xBD, OP_LDA, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0xB9, OP_LDA, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0xA1, OP_LDA, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0xB1, OP_LDA, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0xA2, OP_LDX, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xA6, OP_LDX, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xB6, OP_LDX, ADDR_ZEROPAGEY, 2, 4);
  DEF_INST(0xAE, OP_LDX, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0xBE, OP_LDX, ADDR_ABSOLUTEY, 3, 4);

  DEF_INST(0xA0, OP_LDY, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xA4, OP_LDY, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xB4, OP_LDY, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0xAC, OP_LDY, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0xBC, OP_LDY, ADDR_ABSOLUTEX, 3, 4);

  DEF_INST(0x85, OP_STA, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x95, OP_STA, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x8D, OP_STA, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x9D, OP_STA, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x99, OP_STA, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0x81, OP_STA, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0x91, OP_STA, ADDR_INDIRECTY, 2, 5);

  DEF_INST(0x86, OP_STX, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x96, OP_STX, ADDR_ZEROPAGEY, 2, 4);
  DEF_INST(0x8E, OP_STX, ADDR_ABSOLUTE, 3, 4);

  DEF_INST(0x84, OP_STY, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x94, OP_STY, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x8C, OP_STY, ADDR_ABSOLUTE, 3, 4);

  DEF_INST(0xAA, OP_TAX, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x8A, OP_TXA, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xA8, OP_TAY, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x98, OP_TYA, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x9A, OP_TXS, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xBA, OP_TSX, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x48, OP_PHA, ADDR_IMPLIED, 1, 3);
  DEF_INST(0x68, OP_PLA, ADDR_IMPLIED, 1, 4);
  DEF_INST(0x08, OP_PHP, ADDR_IMPLIED, 1, 3);
  DEF_INST(0x28, OP_PLP, ADDR_IMPLIED, 1, 4);
  DEF_INST(0xEA, OP_NOP, ADDR_IMPLIED, 1, 2);

  /*
   *  unofficial opcode
   *  see http://www.textfiles.com/programming/opcod6502.txt
   */
  DEF_INST(0x0b, OP_AAC, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x2b, OP_AAC, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x87, OP_AAX, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x97, OP_AAX, ADDR_ZEROPAGEY, 2, 4);
  DEF_INST(0x83, OP_AAX, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0x8f, OP_AAX, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x6b, OP_ARR, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x4b, OP_ASR, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xab, OP_ATX, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x9f, OP_AXA, ADDR_ABSOLUTEY, 3, 5);
  DEF_INST(0x93, OP_AXA, ADDR_INDIRECTY, 2, 6);
  DEF_INST(0xcb, OP_AXS, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xc7, OP_DCP, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0xd7, OP_DCP, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0xcf, OP_DCP, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0xdf, OP_DCP, ADDR_ABSOLUTEX, 3, 7);
  DEF_INST(0xdb, OP_DCP, ADDR_ABSOLUTEY, 3, 7);
  DEF_INST(0xc3, OP_DCP, ADDR_INDIRECTX, 2, 8);
  DEF_INST(0xd3, OP_DCP, ADDR_INDIRECTY, 2, 8);
  DEF_INST(0x04, OP_DOP, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x14, OP_DOP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x34, OP_DOP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x44, OP_DOP, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x54, OP_DOP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x64, OP_DOP, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0x74, OP_DOP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0x80, OP_DOP, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x82, OP_DOP, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x89, OP_DOP, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xc2, OP_DOP, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xd4, OP_DOP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0xe2, OP_DOP, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0xf4, OP_DOP, ADDR_ZEROPAGEX, 2, 4);
  DEF_INST(0xe7, OP_ISC, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0xf7, OP_ISC, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0xef, OP_ISC, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0xff, OP_ISC, ADDR_ABSOLUTEX, 3, 7);
  DEF_INST(0xfb, OP_ISC, ADDR_ABSOLUTEY, 3, 7);
  DEF_INST(0xe3, OP_ISC, ADDR_INDIRECTX, 2, 8);
  DEF_INST(0xf3, OP_ISC, ADDR_INDIRECTY, 2, 8);
  DEF_INST(0x02, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x12, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x22, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x32, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x42, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x52, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x62, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x72, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0x92, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0xb2, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0xd2, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0xf2, OP_KIL, ADDR_IMPLIED, 1, 0);
  DEF_INST(0xbb, OP_LAR, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0xa7, OP_LAX, ADDR_ZEROPAGE, 2, 3);
  DEF_INST(0xb7, OP_LAX, ADDR_ZEROPAGEY, 2, 4);
  DEF_INST(0xaf, OP_LAX, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0xbf, OP_LAX, ADDR_ABSOLUTEY, 3, 4);
  DEF_INST(0xa3, OP_LAX, ADDR_INDIRECTX, 2, 6);
  DEF_INST(0xb3, OP_LAX, ADDR_INDIRECTY, 2, 5);
  DEF_INST(0x1a, OP_NOP, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x3a, OP_NOP, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x5a, OP_NOP, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x7a, OP_NOP, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xda, OP_NOP, ADDR_IMPLIED, 1, 2);
  DEF_INST(0xfa, OP_NOP, ADDR_IMPLIED, 1, 2);
  DEF_INST(0x27, OP_RLA, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x37, OP_RLA, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x2f, OP_RLA, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x3f, OP_RLA, ADDR_ABSOLUTEX, 3, 7);
  DEF_INST(0x3b, OP_RLA, ADDR_ABSOLUTEY, 3, 7);
  DEF_INST(0x23, OP_RLA, ADDR_INDIRECTX, 2, 8);
  DEF_INST(0x33, OP_RLA, ADDR_INDIRECTY, 2, 8);
  DEF_INST(0x67, OP_RRA, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x77, OP_RRA, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x6f, OP_RRA, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x7f, OP_RRA, ADDR_ABSOLUTEX, 3, 7);
  DEF_INST(0x7b, OP_RRA, ADDR_ABSOLUTEY, 3, 7);
  DEF_INST(0x63, OP_RRA, ADDR_INDIRECTX, 2, 8);
  DEF_INST(0x73, OP_RRA, ADDR_INDIRECTY, 2, 8);
  DEF_INST(0xeb, OP_SBC, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x07, OP_SLO, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x17, OP_SLO, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x0f, OP_SLO, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x1f, OP_SLO, ADDR_ABSOLUTEX, 3, 7);
  DEF_INST(0x1b, OP_SLO, ADDR_ABSOLUTEY, 3, 7);
  DEF_INST(0x03, OP_SLO, ADDR_INDIRECTX, 2, 8);
  DEF_INST(0x13, OP_SLO, ADDR_INDIRECTY, 2, 8);
  DEF_INST(0x47, OP_SRE, ADDR_ZEROPAGE, 2, 5);
  DEF_INST(0x57, OP_SRE, ADDR_ZEROPAGEX, 2, 6);
  DEF_INST(0x4f, OP_SRE, ADDR_ABSOLUTE, 3, 6);
  DEF_INST(0x5f, OP_SRE, ADDR_ABSOLUTEX, 3, 7);
  DEF_INST(0x5b, OP_SRE, ADDR_ABSOLUTEY, 3, 7);
  DEF_INST(0x43, OP_SRE, ADDR_INDIRECTX, 2, 8);
  DEF_INST(0x53, OP_SRE, ADDR_INDIRECTY, 2, 8);
  DEF_INST(0x9e, OP_SXA, ADDR_ABSOLUTEY, 3, 5);
  DEF_INST(0x9c, OP_SYA, ADDR_ABSOLUTEX, 3, 5);
  DEF_INST(0x0c, OP_TOP, ADDR_ABSOLUTE, 3, 4);
  DEF_INST(0x1c, OP_TOP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x3c, OP_TOP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x5c, OP_TOP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x7c, OP_TOP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0xdc, OP_TOP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0xfc, OP_TOP, ADDR_ABSOLUTEX, 3, 4);
  DEF_INST(0x8b, OP_XAA, ADDR_IMMEDIATE, 2, 2);
  DEF_INST(0x9b, OP_XAS, ADDR_ABSOLUTEY, 3, 5);
}

uint8_t cpu_fetch(CPU *cpu) {
  return cpubus_read(cpu->bus, cpu->reg.pc++);
}

uint16_t cpu_fetch16(CPU *cpu) {
  uint8_t low = cpubus_read(cpu->bus, cpu->reg.pc++);
  uint8_t high = cpubus_read(cpu->bus, cpu->reg.pc++);
  return ((uint16_t)(high) << 8) | low;
}

uint16_t cpu_fetch_operand(CPU *cpu, int addrmode) {
  switch(addrmode) {
    case ADDR_ACCUMULATOR:
    case ADDR_IMPLIED:
      return 0;
    case ADDR_IMMEDIATE:
    case ADDR_ZEROPAGE:
      return cpu_fetch(cpu);
    case ADDR_RELATIVE: {
      uint8_t offset = cpu_fetch(cpu);
      int addr = (int)(int8_t)offset + (int)cpu->reg.pc;
      return (uint16_t)addr;
    }
    case ADDR_ZEROPAGEX:
      return (cpu_fetch(cpu) + cpu->reg.x) & 0xff;
    case ADDR_ZEROPAGEY:
      return (cpu_fetch(cpu) + cpu->reg.y) & 0xff;
    case ADDR_ABSOLUTE:
      return cpu_fetch16(cpu);
    case ADDR_ABSOLUTEX: {
      uint16_t addr = cpu_fetch16(cpu);
      return addr + cpu->reg.x;
    }
    case ADDR_ABSOLUTEY: {
      uint16_t addr = cpu_fetch16(cpu);
      return addr + cpu->reg.y;
    }
    case ADDR_INDIRECT: {
      uint8_t low = cpu_fetch(cpu);
      uint8_t high = cpu_fetch(cpu);
      uint16_t lowaddr = ((uint16_t)(high) << 8) | low;
      uint16_t highaddr = ((uint16_t)(high) << 8) | ((low + 1) & 0xff);

      uint8_t res_low = cpubus_read(cpu->bus, lowaddr);
      uint8_t res_high = cpubus_read(cpu->bus, highaddr);

      return ((uint16_t)(res_high) << 8) | res_low;
    }
    case ADDR_INDIRECTX: {
      uint8_t low = cpu_fetch(cpu) + cpu->reg.x;
      uint8_t res_low = cpubus_read(cpu->bus, low);
      uint8_t res_high = cpubus_read(cpu->bus, (low + 1) & 0xff);

      return ((uint16_t)(res_high) << 8) | res_low;
    }
    case ADDR_INDIRECTY: {
      uint8_t low = cpu_fetch(cpu);
      uint8_t res_low = cpubus_read(cpu->bus, low);
      uint8_t res_high = cpubus_read(cpu->bus, (low + 1) & 0xff);

      return (((uint16_t)(res_high) << 8) | res_low) + cpu->reg.y;
    }
    default:
      panic("Unknown addressing mode");
  }
}

uint8_t cpu_fetch_data(CPU *cpu, int addrmode) {
  switch(addrmode) {
    case ADDR_ACCUMULATOR:  return cpu->reg.a;
    case ADDR_IMPLIED:      return 0;
    case ADDR_IMMEDIATE:    return cpu_fetch_operand(cpu, addrmode);
    case ADDR_ZEROPAGE:
    case ADDR_RELATIVE:
    case ADDR_ZEROPAGEX:
    case ADDR_ZEROPAGEY:
    case ADDR_ABSOLUTE:
    case ADDR_ABSOLUTEX:
    case ADDR_ABSOLUTEY:
    case ADDR_INDIRECT:
    case ADDR_INDIRECTX:
    case ADDR_INDIRECTY: {
      uint16_t addr = cpu_fetch_operand(cpu, addrmode);
      return cpubus_read(cpu->bus, addr);
    }
    default:
      panic("Unknown addressing mode");
  }
}

void cpu_run(CPU *cpu, int cycle) {
  while(cycle > 0) {
    int c = cpu_step(cpu);
    cycle -= c;
  }
}

int cpu_step(CPU *cpu) {
  static uint64_t cpu_cycle = 0;

  if(cpu->halt) return 0;
  uint8_t code = cpu_fetch(cpu);
  CPUInst inst = code_decoder[code];

  /*
  uint16_t op_pc = cpu->reg.pc;
  log_dbg("%04x %02x ", op_pc, code);
  printf("%s ", inst_dump(inst.op));
  printf("A:%02x X:%02x Y:%02x P:%02x SP:%02x CYCLE:%ld\n",
      cpu->reg.a,
      cpu->reg.x,
      cpu->reg.y,
      cpu->reg.p,
      cpu->reg.sp,
      cpu_cycle);
      */

  int cycle = inst.cycle;

  switch(inst.op) {
    case OP_ADC: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      uint16_t res = (uint16_t)cpu->reg.a + m +
        cpu_get_pflag(cpu, P_STATUS_CARRY);

      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_CARRY, res > 0xff);
      cpu_write_pflag(cpu, P_STATUS_OVERFLOW,
          (cpu->reg.a ^ res) & (m ^ res) & (1 << 7));

      cpu->reg.a = res & 0xff;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);

      break;
    }
    case OP_SBC: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      int res = (int)cpu->reg.a - (int)m -
        (1 - cpu_get_pflag(cpu, P_STATUS_CARRY));

      cpu_write_pflag(cpu, P_STATUS_CARRY, res >= 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_OVERFLOW,
          (cpu->reg.a ^ m) & (cpu->reg.a ^ ((uint8_t)res)) & (1 << 7));

      cpu->reg.a = res & 0xff;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);

      break;
    }
    case OP_AND: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      cpu->reg.a &= m;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));

      break;
    }
    case OP_ORA: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      cpu->reg.a = cpu->reg.a | m;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));

      break;
    }
    case OP_EOR: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      cpu->reg.a = cpu->reg.a ^ m;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));

      break;
    }
    case OP_ASL: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = inst.a == ADDR_ACCUMULATOR ?
        cpu->reg.a :
        cpubus_read(cpu->bus, addr);
      uint8_t res = m << 1;

      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);

      if(inst.a == ADDR_ACCUMULATOR)
        cpu->reg.a = res;
      else
        cpubus_write(cpu->bus, addr, res);

      break;
    }
    case OP_LSR: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = inst.a == ADDR_ACCUMULATOR ?
        cpu->reg.a :
        cpubus_read(cpu->bus, addr);
      uint8_t res = m >> 1;

      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 0) & 1);
      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);

      if(inst.a == ADDR_ACCUMULATOR)
        cpu->reg.a = res;
      else
        cpubus_write(cpu->bus, addr, res);

      break;
    }
    case OP_ROL: {
      uint8_t carry = cpu_get_pflag(cpu, P_STATUS_CARRY);

      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = inst.a == ADDR_ACCUMULATOR ?
        cpu->reg.a :
        cpubus_read(cpu->bus, addr);
      uint8_t res = (m << 1) | carry;

      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);

      if(inst.a == ADDR_ACCUMULATOR)
        cpu->reg.a = res;
      else
        cpubus_write(cpu->bus, addr, res);

      break;
    }
    case OP_ROR: {
      uint8_t carry = cpu_get_pflag(cpu, P_STATUS_CARRY);

      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = inst.a == ADDR_ACCUMULATOR ?
        cpu->reg.a :
        cpubus_read(cpu->bus, addr);
      uint8_t res = (m >> 1) | (carry << 7);

      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 0) & 1);
      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);

      if(inst.a == ADDR_ACCUMULATOR)
        cpu->reg.a = res;
      else
        cpubus_write(cpu->bus, addr, res);

      break;
    }
    case OP_INC: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t res = cpubus_read(cpu->bus, addr) + 1;
      cpubus_write(cpu->bus, addr, res);

      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, res & (1 << 7));

      break;
    }
    case OP_INX: {
      cpu->reg.x++;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.x & (1 << 7));

      break;
    }
    case OP_INY: {
      cpu->reg.y++;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.y == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.y & (1 << 7));

      break;
    }
    case OP_DEC: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t res = cpubus_read(cpu->bus, addr) - 1;
      cpubus_write(cpu->bus, addr, res);

      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, res & (1 << 7));

      break;
    }
    case OP_DEX: {
      cpu->reg.x--;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.x & (1 << 7));

      break;
    }
    case OP_DEY: {
      cpu->reg.y--;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.y == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.y & (1 << 7));

      break;
    }
    case OP_BNE: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(!cpu_get_pflag(cpu, P_STATUS_ZERO)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    } 
    case OP_BEQ: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(cpu_get_pflag(cpu, P_STATUS_ZERO)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    } 
    case OP_BIT: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      uint8_t bit6 = (m >> 6) & 1;
      uint8_t bit7 = (m >> 7) & 1;

      cpu_write_pflag(cpu, P_STATUS_ZERO, (cpu->reg.a & m) == 0);
      cpu_write_pflag(cpu, P_STATUS_OVERFLOW, bit6);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, bit7);

      break;
    }
    case OP_BCS: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(cpu_get_pflag(cpu, P_STATUS_CARRY)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    } 
    case OP_BCC: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(!cpu_get_pflag(cpu, P_STATUS_CARRY)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    }
    case OP_BMI: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(cpu_get_pflag(cpu, P_STATUS_NEGATIVE)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    }
    case OP_BVS: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(cpu_get_pflag(cpu, P_STATUS_OVERFLOW)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    } 
    case OP_BVC: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(!cpu_get_pflag(cpu, P_STATUS_OVERFLOW)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    } 
    case OP_BPL: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      if(!cpu_get_pflag(cpu, P_STATUS_NEGATIVE)) {
        cycle++;
        cpu->reg.pc = addr;
      }

      break;
    }
    case OP_JSR: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint16_t pc = cpu->reg.pc - 1;
      cpu_stack_push(cpu, pc >> 8);
      cpu_stack_push(cpu, pc & 0xff);
      cpu->reg.pc = addr;
      break;
    }
    case OP_RTS: {
      uint8_t low = cpu_stack_pop(cpu);
      uint8_t high = cpu_stack_pop(cpu);
      uint16_t addr = ((uint16_t)high << 8) | low;
      cpu->reg.pc = addr + 1;
      break;
    }
    case OP_JMP:
      cpu->reg.pc = cpu_fetch_operand(cpu, inst.a);
      break;
    case OP_CMP: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      uint8_t res = cpu->reg.a - m;

      cpu_write_pflag(cpu, P_STATUS_CARRY, cpu->reg.a >= m);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == m);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);
      break;
    }
    case OP_CPX: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      uint8_t res = cpu->reg.x - m;

      cpu_write_pflag(cpu, P_STATUS_CARRY, cpu->reg.x >= m);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == m);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);

      break;
    }
    case OP_CPY: {
      uint8_t m = cpu_fetch_data(cpu, inst.a);
      uint8_t res = cpu->reg.y - m;

      cpu_write_pflag(cpu, P_STATUS_CARRY, cpu->reg.y >= m);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.y == m);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);

      break;
    }
    case OP_BRK:
      cpu_interrupt(cpu, BRK);
      break;
    case OP_RTI: {
      cpu->reg.p = cpu_stack_pop(cpu);
      uint8_t low = cpu_stack_pop(cpu);
      uint8_t high = cpu_stack_pop(cpu);
      cpu->reg.pc = ((uint16_t)high << 8) | low;
      break;
    }
    case OP_CLD:
      cpu_write_pflag(cpu, P_STATUS_DECIMAL, 0);
      break;
    case OP_SED:
      cpu_write_pflag(cpu, P_STATUS_DECIMAL, 1);
      break;
    case OP_CLI:
      cpu_write_pflag(cpu, P_STATUS_IRQ, 0);
      break;
    case OP_SEI:
      cpu_write_pflag(cpu, P_STATUS_IRQ, 1);
      break;
    case OP_CLV:
      cpu_write_pflag(cpu, P_STATUS_OVERFLOW, 0);
      break;
    case OP_CLC:
      cpu_write_pflag(cpu, P_STATUS_CARRY, 0);
      break;
    case OP_SEC:
      cpu_write_pflag(cpu, P_STATUS_CARRY, 1);
      break;
    case OP_LDA: {
      cpu->reg.a = cpu_fetch_data(cpu, inst.a);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));
      break;
    }
    case OP_LDX: {
      cpu->reg.x = cpu_fetch_data(cpu, inst.a);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.x & (1 << 7));
      break;
    }
    case OP_LDY: {
      cpu->reg.y = cpu_fetch_data(cpu, inst.a);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.y == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.y & (1 << 7));
      break;
    }
    case OP_STA: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      cpubus_write(cpu->bus, addr, cpu->reg.a);
      break;
    }
    case OP_STX: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      cpubus_write(cpu->bus, addr, cpu->reg.x);
      break;
    }
    case OP_STY: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      cpubus_write(cpu->bus, addr, cpu->reg.y);
      break;
    }
    case OP_TAX:
      cpu->reg.x = cpu->reg.a;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.x >> 7) & 1);

      break;
    case OP_TAY:
      cpu->reg.y = cpu->reg.a;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.y == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.y >> 7) & 1);

      break;
    case OP_TXA:
      cpu->reg.a = cpu->reg.x;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.a >> 7) & 1);

      break;
    case OP_TYA:
      cpu->reg.a = cpu->reg.y;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.a >> 7) & 1);

      break;
    case OP_TXS:
      cpu->reg.sp = cpu->reg.x;
      break;
    case OP_TSX:
      cpu->reg.x = cpu->reg.sp;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.x >> 7) & 1);

      break;
    case OP_PHA:
      cpu_stack_push(cpu, cpu->reg.a);
      break;
    case OP_PLA:
      cpu->reg.a = cpu_stack_pop(cpu);

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.a) & (1 << 7));

      break;
    case OP_PHP:
      cpu_stack_push(cpu, cpu->reg.p);
      break;
    case OP_PLP:
      cpu->reg.p = cpu_stack_pop(cpu);
      break;
    case OP_NOP: break;
    case OP_DOP:
      (void)cpu_fetch_operand(cpu, inst.a); // discard result
      break;
    case OP_TOP:
      (void)cpu_fetch_operand(cpu, inst.a); // discard result
      break;
    case OP_KIL:
      cpu->halt = true;
      break;
    case OP_LAX:
      cpu->reg.a = cpu_fetch_data(cpu, inst.a);
      cpu->reg.x = cpu->reg.a;

      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.x == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (cpu->reg.x >> 7) & 1);
      break;
    case OP_AAX: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t res = cpu->reg.x & cpu->reg.a;
      cpubus_write(cpu->bus, addr, res);
      break;
    }
    case OP_DCP: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = cpubus_read(cpu->bus, addr) - 1;
      cpubus_write(cpu->bus, addr, m);
      uint8_t res = cpu->reg.a - m;

      cpu_write_pflag(cpu, P_STATUS_CARRY, cpu->reg.a >= m);
      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);
      break;
    }
    case OP_ISC: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = cpubus_read(cpu->bus, addr) + 1;
      cpubus_write(cpu->bus, addr, m);
      int res = (int)cpu->reg.a - (int)m -
        (1 - cpu_get_pflag(cpu, P_STATUS_CARRY));

      cpu_write_pflag(cpu, P_STATUS_ZERO, res == 0);
      cpu_write_pflag(cpu, P_STATUS_CARRY, res >= 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (res >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_OVERFLOW,
          (cpu->reg.a ^ m) & (cpu->reg.a ^ ((uint8_t)res)) & (1 << 7));

      cpu->reg.a = res & 0xff;
      break;
    }
    case OP_SLO: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = cpubus_read(cpu->bus, addr);
      uint8_t res = m << 1;
      cpubus_write(cpu->bus, addr, res);
      cpu->reg.a |= res;

      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));
      break;
    }
    case OP_RLA: {
      uint8_t carry = cpu_get_pflag(cpu, P_STATUS_CARRY);
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = cpubus_read(cpu->bus, addr);
      uint8_t res = (m << 1) | carry;
      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 7) & 1);
      cpubus_write(cpu->bus, addr, res);

      cpu->reg.a &= res;
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));
      break;
    }
    case OP_SRE: {
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = cpubus_read(cpu->bus, addr);
      uint8_t res = m >> 1;
      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 0) & 1);
      cpubus_write(cpu->bus, addr, res);

      cpu->reg.a ^= res;
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, cpu->reg.a & (1 << 7));
      break;
    }
    case OP_RRA: {
      uint8_t carry = cpu_get_pflag(cpu, P_STATUS_CARRY);
      uint16_t addr = cpu_fetch_operand(cpu, inst.a);
      uint8_t m = cpubus_read(cpu->bus, addr);
      uint8_t res = (m >> 1) | (carry << 7);

      cpu_write_pflag(cpu, P_STATUS_CARRY, (m >> 0) & 1);
      cpubus_write(cpu->bus, addr, res);

      uint16_t adcres = (uint16_t)cpu->reg.a + res + cpu_get_pflag(cpu, P_STATUS_CARRY);

      cpu_write_pflag(cpu, P_STATUS_NEGATIVE, (adcres >> 7) & 1);
      cpu_write_pflag(cpu, P_STATUS_CARRY, adcres > 0xff);
      cpu_write_pflag(cpu, P_STATUS_OVERFLOW,
          (cpu->reg.a ^ adcres) & (res ^ adcres) & (1 << 7));

      cpu->reg.a = adcres & 0xff;
      cpu_write_pflag(cpu, P_STATUS_ZERO, cpu->reg.a == 0);
      break;
    }
    default:
      panic("Unhandled opcode: %s", inst_dump(inst.op));
  }
  cpu_cycle += cycle;

  return cycle;
}

static char *inst_dump(uint8_t op) {
  switch(op) {
#define OP(op) case OP_ ## op: return #op;
#include "cpu/opcode.h"
#undef OP
    default:     return "???";
  }
}
