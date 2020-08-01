#include "apu/triangle.h"
#include "apu/lengthcounter.h"

/* see https://wiki.nesdev.com/w/index.php/APU_Triangle */
const uint8_t triangle_seq[32] = {
  15, 14, 13, 12, 11, 10, 9, 8, 7, 6,  5,  4,  3,  2,  1,  0,
   0,  1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

void triangle_write(struct triangle *tri, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 0x0:
      /* $4008 CRRR RRRR */
      tri->linear_cnt = data & 0x7f;
      tri->halt = (data >> 7) & 0x1;
      break;
    case 0x1: /* $4009 unused */ break;
    case 0x2:
      /* $400A LLLL LLLL */
      tri->reload = tri->timer = (tri->timer & 0x700) | data;
      break;
    case 0x3:
      /* $400B llll lHHH */
      tri->reload = tri->timer = (tri->timer & 0xff) | ((data & 0x7) << 8);
      tri->len_cnt = length_counter[data >> 3];
      tri->linear_reload = true;
      break;
  }
}

void linear_counter_clock(struct triangle *tri) {
  ;
}

void triangle_timer_clock(struct triangle *tri) {
  bool trigger = false;
  if(tri->timer == 0) {
    trigger = true;
    tri->timer = tri->reload;
  }
  else {
    tri->timer--;
  }

  if(trigger)
    tri->sequence = (tri->sequence + 1) & 0x1f;
}

int triangle_output(struct triangle *tri) {
  int s = triangle_seq[tri->sequence];
}
