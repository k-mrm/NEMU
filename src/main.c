#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
#include "log/log.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        panic("invalid arguments");
    }

    Cassette cassette;
    if(read_cassette(&cassette, argv[1])) return 1;
    NEMU nes;
    nes.cassette = &cassette;
    int status = nemu_start(&nes, &argc, argv);

    return status;
}
