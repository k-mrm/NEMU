#ifndef NEMU_CPU_REG_H
#define NEMU_CPU_REG_H

#include <stdint.h>
#include "cpu/cpu.h"

#define cpu_set_pflag(cpu, flag)   ((cpu)->reg.p |= (flag))
#define cpu_unset_pflag(cpu, flag) ((cpu)->reg.p &= ~(flag))
#define cpu_get_pflag(cpu, flag)   ((cpu)->reg.p & (flag))

#define sp_actual(sp)       ((uint16_t)(sp) | 0x01 << 8)
#define sp_actual2raw(sp)   ((uint8_t)((sp) & 0xff))

#define cpu_get_a(cpu)      ((cpu)->reg.a)
#define cpu_get_x(cpu)      ((cpu)->reg.x)
#define cpu_get_y(cpu)      ((cpu)->reg.y)
#define cpu_get_pc(cpu)     ((cpu)->reg.pc)
#define cpu_get_sp_raw(cpu) ((cpu)->reg.sp)
#define cpu_get_sp(cpu)     sp_actual((cpu)->reg.sp)
#define cpu_get_p(cpu)      ((cpu)->reg.p)

#define cpu_set_a(cpu, v)       ((cpu)->reg.a = (v))
#define cpu_set_x(cpu, v)       ((cpu)->reg.x = (v))
#define cpu_set_y(cpu, v)       ((cpu)->reg.y = (v))
#define cpu_set_pc(cpu, v)      ((cpu)->reg.pc = (v))
#define cpu_set_sp_raw(cpu, v)  ((cpu)->reg.sp = (v))
#define cpu_set_sp(cpu, v)      ((cpu)->reg.sp = sp_actual2raw(v))
#define cpu_set_p(cpu, v)       ((cpu)->reg.p = (v))

#endif
