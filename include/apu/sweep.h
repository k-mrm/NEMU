#ifndef NEMU_APU_SWEEP_H
#define NEMU_APU_SWEEP_H

#include <stdint.h>

struct sweepunit {
  uint8_t shift;
  uint8_t period;
  bool neg: 1;
  bool enabled: 1;
  bool reloadf: 1;
};

#endif

