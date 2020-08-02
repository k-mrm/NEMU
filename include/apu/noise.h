#ifndef NEMU_APU_NOISE_H
#define NEMU_APU_NOISE_H

#include <stdint.h>
#include "apu/envelope.h"

struct noise {
  struct envelope eg;
};

#endif
