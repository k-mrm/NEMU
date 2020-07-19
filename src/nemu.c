#include "nemu.h"
#include "cpu/register.h"

static void nemu_init(int argc, char **argv, NEMU *nes) {
  cpu_define_inst();
  cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->cpu.apu, nes->cassette, &nes->pad);
  cpu_init(&nes->cpu, &nes->cpubus);
  ppubus_init(&nes->ppubus, nes->cassette);
  ppu_init(&nes->ppu, &nes->ppubus);
  gui_init(&nes->gui);
  joypad_init(&nes->pad);
  audio_init(argc, argv, &nes->audio);
  apu_init(&nes->cpu.apu);
}

int nemu_boot(int argc, char **argv, NEMU *nes, Cassette *cas) {
  nes->cassette = cas;
  int nmi = 0;
  nemu_init(argc, argv, nes);
  cpu_interrupt(&nes->cpu, RESET);

#ifdef CPU_DEBUG
  nes->cpu.reg.pc = 0xc000;
#endif

  for(;;) {
    request_frame(&nes->gui); /* 60 FPS */
    int pcycle = 0;
    /* draw 1frame */
    while(pcycle < 262 * 341) {
      int cycle = cpu_step(&nes->cpu);
      if(nes->ppu.dma_write_flag) {
        cycle += 513;
        nes->ppu.dma_write_flag = 0;
      }
      pcycle += cycle * 3;
      ppu_step(&nes->ppu, nes->screen, &nmi, cycle * 3);
      if(nmi) {
        cpu_interrupt(&nes->cpu, NMI);
        nmi = 0;
      }
      int irq = apu_step(&nes->cpu.apu, &nes->audio, cycle);
      if(irq) cpu_interrupt(&nes->cpu, IRQ);
    }
    gui_render(nes->screen);

    memset(nes->screen, 0, sizeof(ALLEGRO_VERTEX) * 240 * 256);
  }

  return 0;
}
