#ifndef NEMU_CPU_REG_H
#define NEMU_CPU_REG_H

#include <stdint.h>
#include "cpu/cpu.h"

#define cpu_set_pflag(cpu, flag)   (cpu)->reg.p |= (flag)
#define cpu_unset_pflag(cpu, flag) (cpu)->reg.p &= ~(flag)
#define cpu_get_pflag(cpu, flag)   (((cpu)->reg.p & (flag)) != 0)

#define cpu_write_pflag(cpu, flag, boolean) \
    if(boolean) {   \
        cpu_set_pflag(cpu, flag);   \
    }   \
    else {  \
        cpu_unset_pflag(cpu, flag); \
    }

#define sp_actual(sp)       ((uint16_t)(sp) | (0x01 << 8))

#define cpu_stackptr(cpu)     sp_actual((cpu)->reg.sp)


#endif
