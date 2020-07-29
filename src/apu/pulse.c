#include <stdio.h>
#include <string.h>
#include "audio/audio.h"
#include "apu/pulse.h"
#include "apu/lengthcounter.h"

/* see https://wiki.nesdev.com/w/index.php/APU_Pulse */
const uint8_t pulse_seq[4][8] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1},
};

void pulse_write(struct pulse *pulse, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 0x0:
      /* $4000/$4004 DDLC VVVV */
      pulse->duty = (data & 0xc0) >> 6;
      pulse->eg.volume = data & 0xf;
      pulse->eg.constant = data & 0x10;
      pulse->eg.loop = pulse->halt = data & 0x20;
      break;
    case 0x1:
      /* $4001/$4005 EPPP NSSS */
      break;
    case 0x2:
      /* $4002/$4006 LLLL LLLL */
      pulse->reload = pulse->timer = (pulse->timer & 0x700) | data;
      break;
    case 0x3:
      /* $4003/$4007 llll lHHH */
      pulse->reload = pulse->timer = (pulse->timer & 0xff) | ((data & 0x7) << 8);
      pulse->len_cnt = length_counter[data >> 3];
      pulse->eg.start = true;
      break;
    default: break;
  }
}

void pulse_timer_clock(struct pulse *pulse) {
  bool trigger = false;
  if(pulse->timer == 0) {
    trigger = true;
    pulse->timer = pulse->reload;
  }
  else {
    pulse->timer--;
  }

  if(trigger)
    pulse->sequence = (pulse->sequence + 1) & 0x7;
}

int pulse_output(struct pulse *pulse) {
  int p = pulse_seq[pulse->duty][pulse->sequence];
  if(pulse->len_cnt == 0 || !p)
    return 0;

  return pulse->eg.constant? pulse->eg.volume : pulse->eg.decay;
}
