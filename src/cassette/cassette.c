#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cassette/cassette.h"
#include "log/log.h"

static void cassette_dump(Cassette *cassette) {
  printf("prgrom: %#lx\n", cassette->nprgrom_byte);
  printf("chrrom: %#lx\n", cassette->nchrrom_byte);

  /*
     puts("prgrom:");
     for(int i = 0; i < cassette->nprgrom_byte; ++i) {
     printf("%02x ", cassette->prgrom[i]);
     }
     */
  puts("chrrom:");
  for(int i = 0; i < cassette->nchrrom_byte; ++i) {
    if(cassette->chrrom[i] != 0) {
      printf("no 0 %d ", i);
    }
    printf("%x ", cassette->chrrom[i]);
  }
  puts("");
}

static int parse_ines_format(Cassette *cassette, unsigned char *ines) {
  if(memcmp(ines, "NES\x1A", 4)) {
    panic("This file is not NES format");
  }
  size_t nprgrom_byte = ines[4] * 16384;
  size_t nchrrom_byte = ines[5] * 8192;
  unsigned char flag6 = ines[6];
  unsigned char flag7 = ines[7];
  unsigned char flag8 = ines[8];
  unsigned char flag9 = ines[9];
  unsigned char flag10 = ines[10];

  int has_trainer = flag6 & 0x04;
  size_t ntrainer_byte = has_trainer ? 512 : 0;
  size_t prgrom_base = HEADER_BYTE + ntrainer_byte;
  size_t chrrom_base = prgrom_base + nprgrom_byte;

  cassette->nprgrom_byte = nprgrom_byte;
  cassette->nchrrom_byte = nchrrom_byte;
  memcpy(cassette->prgrom, ines + prgrom_base, nprgrom_byte);
  memcpy(cassette->chrrom, ines + chrrom_base, nchrrom_byte);

  cassette_dump(cassette);

  return 0;
}

int read_cassette(Cassette *cassette, const char *fname) {
  int exitcode = 1;
  FILE *cas = fopen(fname, "r");
  if(!cas) {
    panic("cannot open file: %s", fname);
    return 1;
  }

  fseek(cas, 0, SEEK_END);
  size_t fsize = ftell(cas);
  fseek(cas, 0, SEEK_SET);
  unsigned char *ines = malloc(sizeof(char) * (fsize + 1));
  if(fread(ines, 1, fsize, cas) < fsize) {
    free(ines);
    panic("Error reading file");
    goto end;
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

uint8_t cassette_read_rom(Cassette *cas, int addr) {
  return cas->prgrom[addr];
}

uint8_t cassette_read_chrrom(Cassette *cas, int addr) {
  return cas->chrrom[addr];
}
