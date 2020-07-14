#include "apu/apu.h"

uint8_t apu_read(APU *apu, uint16_t idx) {
  switch(idx) {
    case 0x15:  return apu->io.status;
    default:    return 0;
  }
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

void apu_step() {
  ;
}
