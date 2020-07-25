#ifndef NEMU_APU_LENGTH_COUNTER_H
#define NEMU_APU_LENGTH_COUNTER_H

#include <stdint.h>
#include <stdbool.h>

/* see https://wiki.nesdev.com/w/index.php/APU_Length_Counter */
extern uint8_t length_counter[32];

void length_counter_clock(uint8_t *, bool, bool);

#endif
