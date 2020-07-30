#ifndef NEMU_PULSE_H
#define NEMU_PULSE_H

#include <stdint.h>
#include <stdbool.h>
#include "apu/envelope.h"
#include "apu/lengthcounter.h"

struct sweepunit {
  uint8_t shift;
  uint8_t period;
  bool neg: 1;
  bool enabled: 1;
  bool reload: 1;
  bool mute: 1;
};

struct pulse {
  struct envelope eg;
  struct sweepunit sweep;
  float freq;
  uint8_t len_cnt;
  uint16_t timer: 11;
  uint16_t reload: 11;
  uint8_t sequence;
  int duty;
  bool halt: 1;
  bool enabled: 1;
};

void pulse_update(struct pulse *);
void pulse_write(struct pulse *, uint16_t, uint8_t);
int pulse_output(struct pulse *);
void pulse_timer_clock(struct pulse *);

void sweepunit_clock(struct pulse *);

#endif
