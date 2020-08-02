#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
#include "log/log.h"

int main(int argc, char **argv) {
  Cassette cassette;
  if(argc != 2) {
    panic("invalid arguments");
  }

  if(read_cassette(&cassette, argv[1])) return 1;

  NEMU nes;
  return nemu_boot(argc, argv, &nes, &cassette);
}
