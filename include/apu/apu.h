#ifndef NEMU_APU_H
#define NEMU_APU_H

#include <stdint.h>

typedef struct APU APU;
struct APU {
  struct {
    uint8_t pulse1_ctrl1;   /* $4000 */
    uint8_t pulse1_ctrl2;   /* $4001 */
    uint8_t pulse1_freq1;   /* $4002 */
    uint8_t pulse1_freq2;   /* $4003 */
    uint8_t pulse2_ctrl1;   /* $4004 */
    uint8_t pulse2_ctrl2;   /* $4005 */
    uint8_t pulse2_freq1;   /* $4006 */
    uint8_t pulse2_freq2;   /* $4007 */
    uint8_t triangle_ctrl;  /* $4008 */
    uint8_t triangle_freq1; /* $400a */
    uint8_t triangle_freq2; /* $400b */
    uint8_t noise_ctrl;     /* $400c */
    uint8_t noise_rand;     /* $400e */
    uint8_t noise_time;     /* $400f */
    uint8_t dmc_ctrl1;      /* $4010 */
    uint8_t dmc_ctrl2;      /* $4011 */
    uint8_t dmc_ctrl3;      /* $4012 */
    uint8_t dmc_ctrl4;      /* $4013 */
    uint8_t status;         /* $4015 */
    uint8_t frame_cnt;      /* $4017 */
  } io;
};

uint8_t apu_read(APU *, uint16_t);
void apu_write(APU *, uint16_t, uint8_t);

#endif
