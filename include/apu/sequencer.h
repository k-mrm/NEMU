#ifndef NEMU_APU_SEQUENCER_H
#define NEMU_APU_SEQUENCER_H

#include <stdint.h>

struct sequencer {
  uint16_t timer: 11;
  uint16_t reload: 11;
  uint8_t seq_idx;
};

void sequencer_8step_clock(struct sequencer *);

#endif
