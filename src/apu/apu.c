#include <stdio.h>
#include <string.h>
#include "apu/apu.h"
#include "audio/audio.h"

/* $4015 ---d nt21 */

/* cpu clock: 1789772.5 Hz */

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
      apu->pulse1.seq.timer = (apu->pulse1.seq.timer & 0x700) | data;
      break;
    }
    case 0x3: {
      /* $4003 llll lHHH */
      apu->pulse1.seq.timer = (apu->pulse1.seq.timer & 0xff) | ((data & 0x7) << 8);
      apu->pulse1.len_cnt = length_counter[data >> 3];
      apu->pulse1.freq = 1789772.0 / (16 * (apu->pulse1.seq.timer + 1));
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
    case 0x15: /* TODO */ break;
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

int apu_clock(APU *apu, Audio *audio) {
  apu->cycle++;
  pulse_update(&apu->pulse1);
  pulse_update(&apu->pulse2);

  /* see https://wiki.nesdev.com/w/index.php/APU_Frame_Counter */
  if(apu->seq_mode) {
    /* 5 step */
    if(apu->cycle == 3728) {
      envelope_clock(&apu->pulse1.eg);
    }
    if(apu->cycle == 7456) {
      envelope_clock(&apu->pulse1.eg);
      length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.is_enable, apu->pulse1.halt);
    }
    if(apu->cycle == 11185) {
      envelope_clock(&apu->pulse1.eg);
    }
    if(apu->cycle == 14914) {
      /* NOP */
    }
    if(apu->cycle == 18640) {
      apu->cycle = 0;
      envelope_clock(&apu->pulse1.eg);
      length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.is_enable, apu->pulse1.halt);
    }

    return 0;
  }
  else {
    /* 4 step */
    if(apu->cycle == 3728) {
      envelope_clock(&apu->pulse1.eg);
    }
    if(apu->cycle == 7456) {
      envelope_clock(&apu->pulse1.eg);
      length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.is_enable, apu->pulse1.halt);
    }
    if(apu->cycle == 11185) {
      envelope_clock(&apu->pulse1.eg);
    }
    if(apu->cycle == 14914) {
      apu->cycle = 0;
      envelope_clock(&apu->pulse1.eg);
      length_counter_clock(&apu->pulse1.len_cnt, apu->pulse1.is_enable, apu->pulse1.halt);

      if(!apu->inhibit_irq) return 1;
    }

    return 0;
  }
}

int apu_step(APU *apu, Audio *audio, int cycle) {
  while(cycle--) {
    apu_clock(apu, audio);
  }
}
