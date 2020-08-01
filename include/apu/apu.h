#ifndef NEMU_APU_H
#define NEMU_APU_H

#include <stdint.h>
#include <stdbool.h>
#include "audio/audio.h"
#include "apu/pulse.h"
#include "apu/triangle.h"

typedef struct APU APU;
struct APU {
  struct pulse pulse1;
  struct pulse pulse2;
  struct triangle tri;
  uint16_t cycle;
  uint16_t fs_cycle;
  bool seq_mode: 1;
  bool inhibit_irq: 1;
};

uint8_t apu_read(APU *, uint16_t);
void apu_write(APU *, uint16_t, uint8_t);
void apu_init(APU *);
int apu_step(APU *, Audio *, int);

#endif
