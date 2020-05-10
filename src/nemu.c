#include "nemu.h"

static void nemu_init(NEMU *nes) {
    cpu_define_inst();
    cpubus_init(&nes->cpubus, &nes->ram, &nes->ppu, &nes->apu, nes->cassette);
    cpu_init(&nes->cpu, &nes->cpubus);
    ppubus_init(&nes->ppubus, nes->cassette);
    ppu_init(&nes->ppu, &nes->ppubus);
}

int nemu_start(NEMU *nes) {
    nemu_init(nes);
    cpu_interrupt(&nes->cpu, RESET);

    printf("%#x\n", nes->cpu.reg.pc);

    for(;;) {
        int cycle = cpu_step(&nes->cpu);
        ppu_step(&nes->ppu, cycle * 3);
    }

    return 0;
}
