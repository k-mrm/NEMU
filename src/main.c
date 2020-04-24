#include <stdio.h>
#include <stdlib.h>
#include "cassette/cassette.h"
#include "cpu/cpu.h"
#include "cpu/bus.h"
#include "cpu/instruction.h"
#include "ram/ram.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "log/log.h"

static void nemu_init(CPU *cpu,
                      CPUBus *bus,
                      RAM *ram,
                      PPU *ppu,
                      APU *apu,
                      Cassette *cas) {
    cpu_define_inst();
    cpubus_init(bus, ram, ppu, apu, cas);
    cpu_init(cpu, bus);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        panic("invalid arguments");
    }

    CPU cpu;
    CPUBus bus;
    RAM ram;
    PPU ppu;
    APU apu;
    Cassette cassette;

    if(read_cassette(&cassette, argv[1])) return 1;
    nemu_init(&cpu, &bus, &ram, &ppu, &apu, &cassette);

    return 0;
}
