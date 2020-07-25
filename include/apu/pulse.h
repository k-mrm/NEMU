#ifndef NEMU_PULSE_H
#define NEMU_PULSE_H

#include <stdint.h>
#include <stdbool.h>
#include "apu/envelope.h"
#include "apu/sequencer.h"
#include "apu/lengthcounter.h"
#include "apu/sweep.h"

enum pulseduty {
  DUTY_12_5,
  DUTY_25_0,
  DUTY_50_0,
  DUTY_75_0,
};

struct pulse {
  struct envelope eg;
  struct sweepunit sweep;
  struct sequencer seq;
  float freq;
  uint8_t len_cnt;
  enum pulseduty duty;
  bool halt: 1;
  bool is_enable: 1;
};

void pulse_update(struct pulse *);
void pulse_write(struct pulse *, uint16_t, uint8_t);

#endif
