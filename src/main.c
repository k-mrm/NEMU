#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
#include "log/log.h"

int main(int argc, char **argv) {
  Cassette cassette;

#ifdef CPU_DEBUG
  if(read_cassette(&cassette, "roms/nestest.nes")) return 1;
#else
  if(argc != 2) {
    panic("invalid arguments");
  }

  if(read_cassette(&cassette, argv[1])) return 1;
#endif
  NEMU nes;
  nes.cassette = &cassette;
  int status = nemu_start(&nes, &argc, argv);

  return status;
}
