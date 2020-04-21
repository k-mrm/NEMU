#include <stdio.h>
#include <stdlib.h>

#include "cassette/cassette.h"
#include "cpu/cpu.h"
#include "cpu/instruction.h"
#include "log/log.h"

static int read_cassette(const char *fname) {
    int exitcode = 1;
    FILE *cas = fopen(fname, "r");
    if(!cas) {
        nemu_error("cannot open file: %s", fname);
        return 1;
    }

    fseek(cas, 0, SEEK_END);
    size_t fsize = ftell(cas);
    fseek(cas, 0, SEEK_SET);
    unsigned char *ines = malloc(sizeof(char) * (fsize + 1));
    if(fread(ines, 1, fsize, cas) < fsize) {
        nemu_error("Error reading file");
        free(ines);
        goto end;
    }

    if(parse_ines_format(ines)) {
        free(ines);
        goto end;
    }

    exitcode = 0;
end:
    fclose(cas);
    return exitcode;
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

    if(read_cassette(argv[1])) return 1;

    nes_init();

    return 0;
}
