#ifndef NEMU_APU_TRIANGLE_H
#define NEMU_APU_TRIANGLE_H

#include <stdint.h>
#include <stdbool.h>

struct triangle {
  uint8_t len_cnt;
  uint8_t linear_cnt;
  uint16_t timer: 11;
  uint16_t reload: 11;
  uint8_t sequence;
  bool halt: 1;
  bool linear_reload: 1;
};

void triangle_write(struct triangle *, uint16_t, uint8_t);

#endif
