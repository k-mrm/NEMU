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

void nemu_close(NEMU *nes) {
  gui_close(&nes->gui);
  audio_close(&nes->audio);
}

void nemu_boot(int argc, char **argv, NEMU *nes, Cassette *cas) {
  nes->cassette = cas;
  nemu_init(argc, argv, nes);
  cpu_reset(&nes->cpu);

  for(;;) {
    if(!request_frame(&nes->gui)) // 60 FPS
      break;
    int pcycle = 0;
    /* draw 1frame */
    while(pcycle < 262 * 341) {
      int ccycle = cpu_step(&nes->cpu);
      if(nes->ppu.dma_write_flag) {
        ccycle += 513;
        nes->ppu.dma_write_flag = 0;
      }
      pcycle += ccycle * 3;
      int nmi = ppu_step(&nes->ppu, nes->screen, ccycle * 3);
      if(nmi) cpu_interrupt(&nes->cpu, NMI);

      apu_step(&nes->cpu.apu, &nes->audio, ccycle);
      if(nes->cpu.apu.irq) cpu_interrupt(&nes->cpu, IRQ);
    }
    gui_render(nes->screen);
    audio_update(&nes->audio);
  }
}
