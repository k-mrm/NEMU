#include <stdio.h>
#include <string.h>

#include "cassette/cassette.h"
#include "log/log.h"

Cassette cassette;

static void cassette_dump() {
    printf("prgrom: %zu\n", cassette.nprgrom_byte);
    printf("chrrom: %zu\n", cassette.nchrrom_byte);
}

int parse_ines_format(unsigned char *ines) {
    if(memcmp(ines, "NES\x1A", 4)) {
        nemu_error("This file is not NES format");
        return 1;
    }
    size_t nprgrom_byte = ines[4] * 16384;
    size_t nchrrom_byte = ines[5] * 8192;
    unsigned char flag6 = ines[6];
    unsigned char flag7 = ines[7];
    unsigned char flag8 = ines[8];
    unsigned char flag9 = ines[9];
    unsigned char flag10 = ines[10];

    int has_trainer = flag6 & 0x04;
    size_t trainer_byte = has_trainer ? 512 : 0;
    size_t prgrom_base = HEADER_BYTE + trainer_byte;
    size_t chrrom_base = prgrom_base + nprgrom_byte;

    cassette.nprgrom_byte = nprgrom_byte;
    cassette.nchrrom_byte = nchrrom_byte;
    cassette.prgrom = ines + prgrom_base;
    cassette.chrrom = ines + chrrom_base;

    cassette_dump();

    return 0;
}
