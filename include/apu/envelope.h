#ifndef NEMU_APU_ENVELOPE_H
#define NEMU_APU_ENVELOPE_H

#include <stdint.h>
#include <stdbool.h>

struct envelope {
  uint8_t volume;
  uint8_t decay;
  uint8_t divider;
  bool start: 1;
  bool loop: 1;
  bool constant: 1;
};

void envelope_clock(struct envelope *);

#endif
