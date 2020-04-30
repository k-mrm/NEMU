#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
#include "log/log.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        panic("invalid arguments");
    }

    NEMU nes;
    int status = nemu_start(&nes, argv[1]);

    return status;
}
