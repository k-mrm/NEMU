#include <stdio.h>
#include <string.h>
#include "audio/audio.h"
#include "apu/pulse.h"

uint8_t pulse_seq[4][8] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1},
};

void pulse_update(struct pulse *pulse) {
  ;
}

