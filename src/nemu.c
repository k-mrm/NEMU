#include "nemu.h"
#include "cpu/register.h"

static void nemu_init(NEMU *nes, int *argc, char **argv) {
  cpu_define_inst();
  cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->apu, nes->cassette, &nes->pad);
  cpu_init(&nes->cpu, &nes->cpubus);
  ppubus_init(&nes->ppubus, nes->cassette);
  ppu_init(&nes->ppu, &nes->ppubus);
  gui_init(&nes->gui);
  joypad_init(&nes->pad);
}

int nemu_start(NEMU *nes, int *argc, char **argv) {
  int nmi = 0;
  int f = 0;
  int lpf;
  nemu_init(nes, argc, argv);
  cpu_interrupt(&nes->cpu, RESET);

#ifdef CPU_DEBUG
  nes->cpu.reg.pc = 0xc000;
#endif

  for(;;) {
    request_frame(&nes->gui);
    lpf = 262;
    /* draw 1frame */
    while(lpf--) {
      ppu_step(&nes->ppu, nes->screen, &nmi);
      cpu_run(&nes->cpu, 341 / 3);
      if(nmi) {
        cpu_interrupt(&nes->cpu, NMI);
        nmi = 0;
      }
    }
    gui_render(&nes->gui, nes->screen);
#ifdef CPU_DEBUG
    // printf("@c002 %d\n", cpubus_read(nes->cpu.bus, 0xc002));
    // printf("@c003 %d\n", cpubus_read(nes->cpu.bus, 0xc003));
#endif
  }

  return 0;
}
