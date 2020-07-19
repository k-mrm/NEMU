#include <stdio.h>
#include <string.h>
#include "apu/apu.h"
#include "audio/audio.h"

#define pulse1_timer(apu) (((uint16_t)apu->io.pulse1_freq2 & 0x7) << 8 | apu->io.pulse1_freq1)

/* $4000/$4004 DDLC VVVV */
#define pulse1_duty(apu) (((apu)->io.pulse1_ctrl1 & 0xc0) >> 6)
#define pulse2_duty(apu) (((apu)->io.pulse2_ctrl1 & 0xc0) >> 6)

/* $4015 ---d nt21 */
#define is_enable_pulse1(apu) ((apu)->io.status & 0x1)
#define is_enable_pulse2(apu) ((apu)->io.status & 0x2)
#define is_enable_triangle(apu) ((apu)->io.status & 0x4)
#define is_enable_noise(apu) ((apu)->io.status & 0x8)
#define is_enable_dmc(apu) ((apu)->io.status & 0x10)

/* $4017 MI-- ---- */
#define seq_mode(apu)   (((apu)->io.frame_cnt >> 7) & 0x1)

uint8_t pulse_seq[4][8] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1},
};

/* see https://wiki.nesdev.com/w/index.php/APU_Length_Counter */
uint8_t length_counter[32] = {
  10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,
  12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
};

/* see https://wiki.nesdev.com/w/index.php/APU#Status_.28.244015.29 */
uint8_t apu_read(APU *apu, uint16_t idx) {
  /*
  if(idx == 0x15) return apu->io.status;
  */
  return 0;
}

void apu_write(APU *apu, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 0x0:   apu->io.pulse1_ctrl1 = data; break;
    case 0x1:   apu->io.pulse1_ctrl2 = data; break;
    case 0x2:   apu->io.pulse1_freq1 = data; break;
    case 0x3:   apu->io.pulse1_freq2 = data; break;
    case 0x4:   apu->io.pulse2_ctrl1 = data; break;
    case 0x5:   apu->io.pulse2_ctrl2 = data; break;
    case 0x6:   apu->io.pulse2_freq1 = data; break;
    case 0x7:   apu->io.pulse2_freq2 = data; break;
    case 0x8:   apu->io.triangle_ctrl = data; break;
    case 0xa:   apu->io.triangle_freq1 = data; break;
    case 0xb:   apu->io.triangle_freq2 = data; break;
    case 0xc:   apu->io.noise_ctrl = data; break;
    case 0xe:   apu->io.noise_rand = data; break;
    case 0xf:   apu->io.noise_time = data; break;
    case 0x10:  apu->io.dmc_ctrl1 = data; break;
    case 0x11:  apu->io.dmc_ctrl2 = data; break;
    case 0x12:  apu->io.dmc_ctrl3 = data; break;
    case 0x13:  apu->io.dmc_ctrl4 = data; break;
    case 0x15:  apu->io.status = data; break;
    case 0x17:  apu->io.frame_cnt = data; break;
    default: break;
  }
}

void apu_init(APU *apu) {
  memset(apu, 0, sizeof(APU));
}

void frame_seq_4step(APU *apu) {
  ;
}

void frame_seq_5step(APU *apu) {
  ;
}

int apu_step(APU *apu, Audio *audio, int cycle) {
  static int a = 0;
  apu->cycle += cycle;
  if(apu->cycle < 7457) return 0;

  apu->cycle -= 7457;
  if(seq_mode(apu)) {
    frame_seq_5step(apu);
  }
  else {
    frame_seq_4step(apu);
  }
  return 0;
}
