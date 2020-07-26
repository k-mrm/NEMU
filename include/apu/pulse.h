#ifndef NEMU_PULSE_H
#define NEMU_PULSE_H

#include <stdint.h>
#include <stdbool.h>
#include "apu/envelope.h"
#include "apu/sequencer.h"
#include "apu/lengthcounter.h"
#include "apu/sweep.h"

struct pulse {
  struct envelope eg;
  struct sweepunit sweep;
  struct sequencer seq;
  float freq;
  uint8_t len_cnt;
  int duty;
  bool halt: 1;
  bool is_enable: 1;
};

void pulse_update(struct pulse *);
void pulse_write(struct pulse *, uint16_t, uint8_t);
void pulse_output(struct pulse *);

#endif
