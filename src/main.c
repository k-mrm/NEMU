#include <stdio.h>
#include <stdlib.h>

#include "cassette/cassette.h"

static unsigned char *read_nes_file(const char *fname) {
    FILE *nes = fopen(fname, "r");
    if(!nes) {
        fprintf(stderr, "cannot open file: %s\n", fname);
        return NULL;
    }

    fseek(nes, 0, SEEK_END);
    size_t fsize = ftell(nes);
    fseek(nes, 0, SEEK_SET);
    unsigned char *rom = malloc(sizeof(char) * (fsize + 1));
    if(fread(rom, 1, fsize, nes) < fsize) {
        fprintf(stderr, "Error reading file\n");
        free(rom);
        rom = NULL;
    }
    fclose(nes);

    return rom;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "invalid arguments\n");
        return 1;
    }

    unsigned char *rom = read_nes_file(argv[1]);
    if(!rom) return 1; 

    if(parse_ines_format(rom)) {
        return 1;
    }

    return 0;
}
