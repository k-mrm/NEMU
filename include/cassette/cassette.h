#ifndef NEMU_CASSETTE_H
#define NEMU_CASSETTE_H

#include <stdio.h>
#include <stdint.h>

#define HEADER_BYTE 16

enum mirroring {
  HORIZONTAL = 0,
  VERTICAL   = 1,
};

typedef struct Cassette Cassette;
struct Cassette {
  size_t nprgrom_byte;
  size_t nchrrom_byte;
  uint8_t prgrom[0x8000];
  uint8_t chrrom[0x2000];
  enum mirroring mirror;
  uint8_t mapper;
};

int read_cassette(Cassette *, const char *);
uint8_t cassette_read_rom(Cassette *, int);
uint8_t cassette_read_chrrom(Cassette *, int);
enum mirroring nes_mirroring(Cassette *);

#endif
