#include <stdio.h>
#include <stdlib.h>

#include "cassette/cassette.h"
#include "cpu/cpu.h"
#include "cpu/instruction.h"
#include "log/log.h"

static unsigned char *read_nes_file(const char *fname) {
    FILE *nes = fopen(fname, "r");
    if(!nes) {
        nemu_error("cannot open file: %s", fname);
        return NULL;
    }

    fseek(nes, 0, SEEK_END);
    size_t fsize = ftell(nes);
    fseek(nes, 0, SEEK_SET);
    unsigned char *rom = malloc(sizeof(char) * (fsize + 1));
    if(fread(rom, 1, fsize, nes) < fsize) {
        nemu_error("Error reading file");
        free(rom);
        rom = NULL;
    }
    fclose(nes);

    return rom;
}

void nes_init() {
    cpu_define_inst();
    cpu_init();
}

int main(int argc, char **argv) {
    if(argc != 2) {
        nemu_error("invalid arguments");
        return 1;
    }

    unsigned char *rom = read_nes_file(argv[1]);
    if(!rom) return 1; 

    nes_init();
    if(parse_ines_format(rom)) return 1;

    return 0;
}
