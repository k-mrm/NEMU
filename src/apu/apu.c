#include <stdio.h>
#include <string.h>
#include "apu/apu.h"
#include "audio/audio.h"

float pulse_table[32];
float tnd_table[204];

/* see https://wiki.nesdev.com/w/index.php/APU#Status_.28.244015.29 */
uint8_t apu_read(APU *apu, uint16_t idx) {
  /*
  if(idx == 0x15) return apu->io.status;
  */
  return 0;
}

void apu_write(APU *apu, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 0x0: case 0x1:
    case 0x2: case 0x3:
      pulse_write(&apu->pulse1, idx, data); break;
    case 0x4: case 0x5:
    case 0x6: case 0x7:
      pulse_write(&apu->pulse2, idx - 0x4, data); break;
    case 0x8:
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13: /* TODO */ break;
    case 0x15:
      /* $4015 ---d nt21 */
      apu->pulse1.enabled = data & 0x1;
      apu->pulse2.enabled = data & 0x2;
      break;
    case 0x17:
      /* $4017 MI-- ---- */
      apu->seq_mode = (data >> 7) & 0x1;
      apu->inhibit_irq = (data >> 6) & 0x1;
      break;
    default: break;
  }
}

void apu_init(APU *apu) {
  memset(apu, 0, sizeof(APU));

  /* see https://wiki.nesdev.com/w/index.php/APU_Mixer#Lookup_Table */
  for(int n = 0; n < 32; ++n)
    pulse_table[n] = 95.52 / (8128.0 / n + 100);
  for(int n = 0; n < 204; ++n)
    tnd_table[n] = 163.67 / (24329.0 / n + 100);
}

int frame_seq_5step(APU *apu) {
  if(apu->cycle == 7457) {
    envelope_clock(&apu->pulse1.eg);
  }
  if(apu->cycle == 14913) {
    envelope_clock(&apu->pulse1.eg);
    length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.enabled, apu->pulse1.halt);
  }
  if(apu->cycle == 22371) {
    envelope_clock(&apu->pulse1.eg);
  }
  if(apu->cycle == 29828) {
    /* NOP */
  }
  if(apu->cycle == 37281) {
    apu->cycle = 0;
    envelope_clock(&apu->pulse1.eg);
    length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.enabled, apu->pulse1.halt);
  }
  return 0;
}

int frame_seq_4step(APU *apu) {
  if(apu->cycle == 7457) {
    envelope_clock(&apu->pulse1.eg);
  }
  if(apu->cycle == 14913) {
    envelope_clock(&apu->pulse1.eg);
    length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.enabled, apu->pulse1.halt);
  }
  if(apu->cycle == 22371) {
    envelope_clock(&apu->pulse1.eg);
  }
  if(apu->cycle == 29828) {
    apu->cycle = 0;
    envelope_clock(&apu->pulse1.eg);
    length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.enabled, apu->pulse1.halt);

    if(!apu->inhibit_irq) return 1;
  }
  return 0;
}

void apu_sample(APU *apu, Audio *audio) {
  int p1 = pulse_output(&apu->pulse1);
  int p2 = pulse_output(&apu->pulse2);
  /* TODO: tnd */
  float out = pulse_table[p1 + p2];
  printf("out: %f\n", out);
}

int apu_clock(APU *apu, Audio *audio) {
  static const int sampling_period = 1789773 / 44100;

  apu->cycle++;

  if(apu->cycle % sampling_period == 0) {
    /* sound */
    apu_sample(apu, audio);
  }

  if(apu->cycle % 2 == 0) { /* APU clock */
    pulse_timer_clock(&apu->pulse1);
    pulse_timer_clock(&apu->pulse2);
  }

  /* see https://wiki.nesdev.com/w/index.php/APU_Mixer */
  if(apu->seq_mode)
    return frame_seq_5step(apu);
  else
    return frame_seq_4step(apu);
}

int apu_step(APU *apu, Audio *audio, int cpucycle) {
  while(cpucycle--) {
    apu_clock(apu, audio);
  }
}
