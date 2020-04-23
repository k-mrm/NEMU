#include <stdio.h>
#include <stdlib.h>

#include "cassette/cassette.h"
#include "cpu/cpu.h"
#include "cpu/instruction.h"
#include "log/log.h"

static void nemu_init(CPU *cpu) {
    cpu_define_inst();
    cpu_init(cpu);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        panic("invalid arguments");
    }

    CPU cpu;
    Cassette cassette;

    if(read_cassette(&cassette, argv[1])) return 1;
    nemu_init(&cpu);

    return 0;
}
