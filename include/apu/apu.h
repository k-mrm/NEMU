#ifndef NEMU_APU_H
#define NEMU_APU_H

#include <stdint.h>
#include "audio/audio.h"

enum pulseduty {
  DUTY_12_5,
  DUTY_25_0,
  DUTY_50_0,
  DUTY_75_0,
};

struct envelope {
  uint8_t volume;
  uint8_t decay;
  uint8_t divider;
  uint8_t start;
};

struct pulse {
  struct envelope eg;
  uint8_t len_cnt;
  uint8_t duty;
  uint8_t halt;
};

typedef struct APU APU;
struct APU {
  struct pulse pulse1;
  struct pulse pulse2;
  uint16_t timer: 11;
  uint16_t cycle;
  uint8_t seq_mode;
  uint8_t is_enable_irq;
};

uint8_t apu_read(APU *, uint16_t);
void apu_write(APU *, uint16_t, uint8_t);
void apu_init(APU *);
int apu_step(APU *, Audio *, int);

#endif
