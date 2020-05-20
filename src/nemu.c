#include "nemu.h"

static void nemu_init(NEMU *nes, int *argc, char **argv) {
  cpu_define_inst();
  cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->apu, nes->cassette);
  cpu_init(&nes->cpu, &nes->cpubus);
  ppubus_init(&nes->ppubus, nes->cassette);
  ppu_init(&nes->ppu, &nes->ppubus);
  gui_init(&nes->gui);
}

int nemu_start(NEMU *nes, int *argc, char **argv) {
  nemu_init(nes, argc, argv);
  cpu_interrupt(&nes->cpu, RESET);

#ifdef CPU_DEBUG
  nes->cpu.reg.pc = 0xc000;
#endif

  for(;;) {
    int cycle = cpu_step(&nes->cpu);
    int draw = ppu_step(&nes->ppu, cycle * 3, nes->screen);
    if(draw) {
      gui_render(&nes->gui, nes->screen);
    }
#ifdef CPU_DEBUG
    // printf("@c002 %d\n", cpubus_read(nes->cpu.bus, 0xc002));
    // printf("@c003 %d\n", cpubus_read(nes->cpu.bus, 0xc003));
#endif
  }

  return 0;
}
