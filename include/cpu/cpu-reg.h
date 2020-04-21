#ifndef NEMU_CPU_REG_H
#define NEMU_CPU_REG_H

#define cpu_set_pflag(p, flag)   ((p) |= (flag))
#define cpu_unset_pflag(p, flag) ((p) &= ~(flag))
#define cpu_get_pflag(p, flag)   ((p) & (flag))

#define cpu_get_a(cpu)   ((cpu).a)
#define cpu_get_x(cpu)   ((cpu).x)
#define cpu_get_y(cpu)   ((cpu).y)
#define cpu_get_pc(cpu)  ((cpu).pc)
#define cpu_get_sp(cpu)  ((cpu).sp)
#define cpu_get_p(cpu)   ((cpu).p)

#define cpu_set_a(cpu, v)   ((cpu).a = (v))
#define cpu_set_x(cpu, v)   ((cpu).x = (v))
#define cpu_set_y(cpu, v)   ((cpu).y = (v))
#define cpu_set_pc(cpu, v)  ((cpu).pc = (v))
#define cpu_set_sp(cpu, v)  ((cpu).sp = (v))
#define cpu_set_p(cpu, v)   ((cpu).p = (v))

#endif
