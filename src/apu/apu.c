#include <stdio.h>
#include <string.h>
#include "apu/apu.h"
#include "audio/audio.h"


/* $4015 ---d nt21 */

/* cpu clock: 1789772.5 */

static uint8_t pulse_seq[4][8] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1},
};

/* see https://wiki.nesdev.com/w/index.php/APU_Length_Counter */
static uint8_t length_counter[32] = {
  10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,
  12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
};

static void envelope_clock(struct envelope *e) {
  if(e->start) {
    e->start = 0;
    e->decay = 15;
    e->divider = e->volume;
  }
  else if(e->divider == 0) {
    e->divider = e->volume;
    e->decay--;

    if(e->decay == 0) {
      if(e->loop)
        e->decay = 15;
    }
    else {
      e->decay--;
    }
  }
  else {
    e->divider--;
  }
}

static void length_counter_clock(uint8_t *c, bool enable, bool halt) {
  if(enable) {
    if(*c && !halt) {
      (*c)--;
    }
  }
  else {
    *c = 0;
  }
}

/* see https://wiki.nesdev.com/w/index.php/APU#Status_.28.244015.29 */
uint8_t apu_read(APU *apu, uint16_t idx) {
  /*
  if(idx == 0x15) return apu->io.status;
  */
  return 0;
}

void apu_write(APU *apu, uint16_t idx, uint8_t data) {
  switch(idx) {
    case 0x0: {
      /* $4000 DDLC VVVV */
      apu->pulse1.duty = (data & 0xc0) >> 6;
      apu->pulse1.eg.volume = data & 0xf;
      apu->pulse1.eg.constant = data & 0x10;
      apu->pulse1.eg.loop = apu->pulse1.halt = data & 0x20;
      break;
    }
    case 0x1: {
      /* $4001 EPPP NSSS */
      break;
    }
    case 0x2: {
      /* $4002 LLLL LLLL */
      apu->pulse1.timer = (apu->pulse1.timer & 0x700) | data;
      break;
    }
    case 0x3: {
      /* $4003 llll lHHH */
      apu->pulse1.timer = (apu->pulse1.timer & 0xff) | ((data & 0x7) << 8);
      apu->pulse1.len_cnt = length_counter[data >> 3];
      break;
    }
    case 0x4: {
      /* $4004 DDLC VVVV */
      apu->pulse2.duty = (data & 0xc0) >> 6;
      apu->pulse2.eg.volume = data & 0xf;
      apu->pulse2.halt = data & 0x20;
      break;
    }
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x15: break;
    case 0x17: {
      /* $4017 MI-- ---- */
      apu->seq_mode = (data >> 7) & 0x1;
      apu->inhibit_irq = (data >> 6) & 0x1;
      break;
    }
    default: break;
  }
}

void apu_init(APU *apu) {
  memset(apu, 0, sizeof(APU));
}

int frame_seq_4step(APU *apu) {
  apu->step++;
  envelope_clock(&apu->pulse1.eg);
  if(apu->step % 2 == 0) {
    length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.is_enable, apu->pulse1.halt);
  }

  if(apu->step == 4) {
    apu->step = 0;

    if(!apu->inhibit_irq) {
      return 1;
    }
  }

  return 0;
}

int frame_seq_5step(APU *apu) {
  apu->step++;
  if(apu->step != 4) {
    envelope_clock(&apu->pulse1.eg);
  }

  if(apu->step == 2 || apu->step == 5) {
    length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.is_enable, apu->pulse1.halt);
  } 

  return 0;
}

int apu_step(APU *apu, Audio *audio, int cycle) {
  static int a = 0;
  apu->cycle += cycle;
  if(apu->cycle < 7457) return 0;

  apu->cycle -= 7457;
  if(apu->seq_mode) {
    return frame_seq_5step(apu);
  }
  else {
    return frame_seq_4step(apu);
  }
}
