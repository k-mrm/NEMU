#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cassette/cassette.h"
#include "log/log.h"

/*
static void cassette_dump(Cassette *cassette) {
  printf("prgrom: %#lx\n", cassette->nprgrom_byte);
  printf("chrrom: %#lx\n", cassette->nchrrom_byte);

  puts("prgrom:");
  for(int i = 0; i < cassette->nprgrom_byte; ++i) {
    printf("%02x ", cassette->prgrom[i]);
  }
  puts("chrrom:");
  for(int i = 0; i < cassette->nchrrom_byte; ++i) {
    printf("%x ", cassette->chrrom[i]);
  }
  puts("");
}
*/

static void mapper_check(int map) {
  switch(map) {
    case 0:   break;
    default:  panic("unsupported mapper: %d", map);
  }
}

static int parse_ines_format(Cassette *cas, unsigned char *ines) {
  if(memcmp(ines, "NES\x1A", 4))
    panic("This file is not NES format");

  size_t nprgrom_byte = ines[4] * 16384;
  size_t nchrrom_byte = ines[5] * 8192;
  unsigned char flag6 = ines[6];
  unsigned char flag7 = ines[7];

  uint8_t mapper_low = flag6 >> 4 & 0xf;
  uint8_t mapper_high = flag7 & 0xf0;
  cas->mapper = mapper_low | mapper_high;
  mapper_check(cas->mapper);

  cas->mirror = flag6 & 0x01;
  int has_trainer = flag6 & 0x04;
  size_t ntrainer_byte = has_trainer? 512: 0;
  size_t prgrom_base = HEADER_BYTE + ntrainer_byte;
  size_t chrrom_base = prgrom_base + nprgrom_byte;

  cas->nprgrom_byte = nprgrom_byte;
  cas->nchrrom_byte = nchrrom_byte;
  memcpy(cas->prgrom, ines + prgrom_base, nprgrom_byte);
  memcpy(cas->chrrom, ines + chrrom_base, nchrrom_byte);

  return 0;
}

int read_cassette(Cassette *cassette, const char *fname) {
  int exitcode = 1;
  FILE *cas = fopen(fname, "r");
  if(!cas) {
    panic("cannot open file: %s", fname);
  }

  fseek(cas, 0, SEEK_END);
  size_t fsize = ftell(cas);
  fseek(cas, 0, SEEK_SET);
  unsigned char *ines = malloc(sizeof(char) * (fsize + 1));
  if(fread(ines, 1, fsize, cas) < fsize) {
    free(ines);
    panic("Error reading file");
  }

  if(parse_ines_format(cassette, ines)) {
    free(ines);
    goto end;
  }

  exitcode = 0;
end:
  fclose(cas);
  return exitcode;
}

enum mirroring nes_mirroring(Cassette *cas) {
  return cas->mirror;
}

uint8_t cassette_read_rom(Cassette *cas, int addr) {
  return cas->prgrom[addr];
}

uint8_t cassette_read_chrrom(Cassette *cas, int addr) {
  return cas->chrrom[addr];
}
