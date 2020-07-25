#include "apu/lengthcounter.h"

uint8_t length_counter[32] = {
  10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,
  12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
};

void length_counter_clock(uint8_t *c, bool enable, bool halt) {
  if(enable) {
    if(*c && !halt) {
      (*c)--;
    }
  }
  else {
    *c = 0;
  }
}
