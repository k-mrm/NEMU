#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
#include "log/log.h"

int main(int argc, char **argv) {
  Cassette cassette;
  char *cname;
#ifdef CPU_DEBUG
  cname = "roms/nestest.nes";
#else
  if(argc != 2) {
    panic("invalid arguments");
  }
  cname = argv[1];
#endif

  if(read_cassette(&cassette, cname)) return 1;

  NEMU nes;
  nes.cassette = &cassette;
  return nemu_start(&nes, &argc, argv);
}
