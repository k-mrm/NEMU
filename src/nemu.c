#include "nemu.h"

static void nemu_init(NEMU *nes) {
    cpu_define_inst();
    cpubus_init(&nes->bus, &nes->ram, &nes->ppu, &nes->apu, &nes->cassette);
    cpu_init(&nes->cpu, &nes->bus);
}

int nemu_start(NEMU *nes, char *cas_file) {
    if(read_cassette(&nes->cassette, cas_file)) return 1;
    nemu_init(nes);
    cpu_interrupt(&nes->cpu, RESET);

    printf("%#x\n", nes->cpu.reg.pc);

    for(;;) {
        cpu_step(&nes->cpu);
        printf("%#x\n", nes->cpu.reg.pc);
    }

    return 0;
}
