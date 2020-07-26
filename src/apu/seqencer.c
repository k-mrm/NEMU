#include <stdbool.h>
#include "apu/sequencer.h"

void sequencer_8step_clock(struct sequencer *seq) {
  bool trigger = false;
  if(seq->timer == 0) {
    trigger = true;
    seq->timer = seq->reload;
  }
  else {
    seq->timer--;
  }

  if(trigger) {
    seq->seq_idx = (seq->seq_idx + 1) & 0x7;
  }
}
