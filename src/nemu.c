#include "nemu.h"
#include "cpu/register.h"

static void nemu_init(NEMU *nes, int *argc, char **argv) {
  cpu_define_inst();
  cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->apu, nes->cassette);
  cpu_init(&nes->cpu, &nes->cpubus);
  ppubus_init(&nes->ppubus, nes->cassette);
  ppu_init(&nes->ppu, &nes->ppubus);
  gui_init(&nes->gui);
}

int nemu_start(NEMU *nes, int *argc, char **argv) {
  int nmi;
  nemu_init(nes, argc, argv);
  cpu_interrupt(&nes->cpu, RESET);

#ifdef CPU_DEBUG
  nes->cpu.reg.pc = 0xc000;
#endif

  int f = 0;
  for(;;) {
    nmi = 0;
    int cycle = cpu_step(&nes->cpu);
    int draw = ppu_step(&nes->ppu, cycle * 3, nes->screen, &nmi);
    if(nmi) {
      cpu_interrupt(&nes->cpu, NMI);
    }
    if(draw) {
      // gui_render_console(&nes->gui, nes->screen);
      // printf("frame %d\n", f++);
    }
#ifdef CPU_DEBUG
    // printf("@c002 %d\n", cpubus_read(nes->cpu.bus, 0xc002));
    // printf("@c003 %d\n", cpubus_read(nes->cpu.bus, 0xc003));
#endif
  }

  return 0;
}
