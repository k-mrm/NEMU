#include "nemu.h"

static void nemu_init(NEMU *nes, int *argc, char **argv) {
    cpu_define_inst();
    cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->apu, nes->cassette);
    cpu_init(&nes->cpu, &nes->cpubus);
    ppubus_init(&nes->ppubus, nes->cassette);
    ppu_init(&nes->ppu, &nes->ppubus);
    gui_init(argc, argv);
}

int nemu_start(NEMU *nes, int *argc, char **argv) {
    nemu_init(nes, argc, argv);
    cpu_interrupt(&nes->cpu, RESET);

    printf("%#x\n", nes->cpu.reg.pc);

    for(;;) {
        int cycle = cpu_step(&nes->cpu);
        int draw = ppu_step(&nes->ppu, cycle * 3, nes->screen);
        if(draw) {
            ppu_render(&nes->ppu, nes->screen);
        }
    }

    return 0;
}
