#include "cpu/cpu.h"
#include "cpu/instruction.h"
#include "cpu/register.h"
#include "log/log.h"

CPUInst code_decoder[256];

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
}

uint8_t cpu_fetch(CPU *cpu) {
    return cpubus_read(cpu->bus, cpu->reg.pc++);
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
        int data = (int)offset + (int)cpu->reg.pc;
        return (uint16_t)data;
    }
    case ADDR_ZEROPAGEX:
        return (cpu_fetch(cpu) + cpu->reg.x) & 0xff;
    case ADDR_ZEROPAGEY:
        return (cpu_fetch(cpu) + cpu->reg.y) & 0xff;
    case ADDR_ABSOLUTE: {
        uint8_t low = cpu_fetch(cpu);
        uint8_t high = cpu_fetch(cpu);
        return ((uint16_t)(high) << 8) | low;
    }
    case ADDR_ABSOLUTEX: {
        uint8_t low = cpu_fetch(cpu);
        uint8_t high = cpu_fetch(cpu);
        return (((uint16_t)(high) << 8) | low) + cpu->reg.x;
    }
    case ADDR_ABSOLUTEY: {
        uint8_t low = cpu_fetch(cpu);
        uint8_t high = cpu_fetch(cpu);
        return (((uint16_t)(high) << 8) | low) + cpu->reg.y;
    }
    case ADDR_INDIRECT: {
        uint8_t low = cpu_fetch(cpu);
        uint8_t high = cpu_fetch(cpu);
        uint16_t addr = ((uint16_t)(high) << 8) | low;

        uint8_t res_low = cpubus_read(cpu->bus, addr);
        uint8_t res_high = cpubus_read(cpu->bus, addr + 1);

        return ((uint16_t)(res_high) << 8) | res_low;
    }
    case ADDR_INDIRECTX: {
        uint8_t low = cpu_fetch(cpu) + cpu->reg.x;
        uint8_t res_low = cpubus_read(cpu->bus, low);
        uint8_t res_high = cpubus_read(cpu->bus, low + 1);

        return ((uint16_t)(res_high) << 8) | res_low;
    }
    case ADDR_INDIRECTY: {
        uint8_t low = cpu_fetch(cpu);
        uint8_t res_low = cpubus_read(cpu->bus, low);
        uint8_t res_high = cpubus_read(cpu->bus, low + 1);

        return (((uint16_t)(res_high) << 8) | res_low) + cpu->reg.y;
    }
    default:
        panic("Unknown addressing mode");
        return 0;   /* unreachable */
    }
}

uint8_t cpu_fetch_data(CPU *cpu, int addrmode) {
    switch(addrmode) {
    case ADDR_ACCUMULATOR:
        return cpu->reg.a;
    case ADDR_IMPLIED:
        return 0;
    case ADDR_IMMEDIATE:
        return cpu_fetch_operand(cpu, addrmode);
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
        return 0;   /* unreachable */
    }
}

int cpu_step(CPU *cpu) {
    uint8_t code = cpu_fetch(cpu);
    CPUInst inst = code_decoder[code];

    int cycle = inst.cycle;

    switch(inst.op) {
    case OP_NOP:
        break;
    case OP_ADC: {
        uint8_t m = cpu_fetch_data(cpu, inst.a);
        uint16_t res = cpu->reg.a + m + cpu_get_pflag(cpu, P_STATUS_NEGATIVE);

        if(res == 0) cpu_set_pflag(cpu, P_STATUS_ZERO);
        if(res & (1 << 7)) cpu_set_pflag(cpu, P_STATUS_NEGATIVE);
        if(res > 0xff)  cpu_set_pflag(cpu, P_STATUS_CARRY);
        if((cpu->reg.a ^ res) & (m ^ res) & (1 << 7))
            cpu_set_pflag(cpu, P_STATUS_OVERFLOW);

        cpu->reg.a = res;

        break;
    }
    case OP_INX: {
        cpu->reg.x++;

        if(cpu->reg.x == 0) cpu_set_pflag(cpu, P_STATUS_ZERO);
        if(cpu->reg.x & (1 << 7)) cpu_set_pflag(cpu, P_STATUS_NEGATIVE);

        break;
    }
    case OP_DEY: {
        cpu->reg.y--;

        if(cpu->reg.y == 0) cpu_set_pflag(cpu, P_STATUS_ZERO);
        if(cpu->reg.y & (1 << 7)) cpu_set_pflag(cpu, P_STATUS_NEGATIVE);

        break;
    }
    case OP_BNE: {
        uint16_t addr = cpu_fetch_operand(cpu, inst.a);
        if(!cpu_get_pflag(cpu, P_STATUS_ZERO)) {
            cpu->reg.pc = addr;
        }

        break;
    } 
    case OP_JMP:
        cpu->reg.pc = cpu_fetch_operand(cpu, inst.a);
        break;
    case OP_LDA: {
        cpu->reg.a = cpu_fetch_data(cpu, inst.a);
        if(cpu->reg.a == 0) cpu_set_pflag(cpu, P_STATUS_ZERO);
        if(cpu->reg.a & (1 << 7)) cpu_set_pflag(cpu, P_STATUS_NEGATIVE);
        break;
    }
    case OP_LDX: {
        cpu->reg.x = cpu_fetch_data(cpu, inst.a);
        if(cpu->reg.x == 0) cpu_set_pflag(cpu, P_STATUS_ZERO);
        if(cpu->reg.x & (1 << 7)) cpu_set_pflag(cpu, P_STATUS_NEGATIVE);
        break;
    }
    case OP_LDY: {
        cpu->reg.y = cpu_fetch_data(cpu, inst.a);
        if(cpu->reg.y == 0) cpu_set_pflag(cpu, P_STATUS_ZERO);
        if(cpu->reg.y & (1 << 7)) cpu_set_pflag(cpu, P_STATUS_NEGATIVE);
        break;
    }
    case OP_SEI:
        cpu_set_pflag(cpu, P_STATUS_IRQ);
        break;
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
    case OP_TXS:
        cpu->reg.sp = cpu->reg.x;
        break;
    default:
        panic("Unhandled opcode: %#x", inst.op);
        break;
    }

    return cycle;
}

