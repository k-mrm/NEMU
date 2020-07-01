#include "nemu.h"
#include "cpu/register.h"

static void nemu_init(NEMU *nes) {
  cpu_define_inst();
  cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->apu, nes->cassette, &nes->pad);
  cpu_init(&nes->cpu, &nes->cpubus);
  ppubus_init(&nes->ppubus, nes->cassette);
  ppu_init(&nes->ppu, &nes->ppubus);
  gui_init(&nes->gui);
  joypad_init(&nes->pad);
}

int nemu_boot(NEMU *nes, Cassette *cas) {
  nes->cassette = cas;
  int nmi = 0;
  int c = 0;
  int lpf;
  nemu_init(nes);
  cpu_interrupt(&nes->cpu, RESET);

#ifdef CPU_DEBUG
  nes->cpu.reg.pc = 0xc000;
#endif

  for(;;) {
    request_frame(&nes->gui);
    int pcycle = 0;
    /* draw 1frame */
    while(pcycle < 262 * 341) {
      int cycle = cpu_step(&nes->cpu);
      c = ppu_step(&nes->ppu, nes->screen, &nmi, cycle * 3);
      pcycle += cycle * 3;
      if(nmi) {
        cpu_interrupt(&nes->cpu, NMI);
        nmi = 0;
      }
    }
    RGB rgb = colors[c];
    al_clear_to_color(al_map_rgb(rgb.r, rgb.g, rgb.b));
    gui_render(nes->screen);

    memset(nes->screen, 0, sizeof(ALLEGRO_VERTEX) * 240 * 256);
#ifdef CPU_DEBUG
    // printf("@c002 %d\n", cpubus_read(nes->cpu.bus, 0xc002));
    // printf("@c003 %d\n", cpubus_read(nes->cpu.bus, 0xc003));
#endif
  }

  return 0;
}
