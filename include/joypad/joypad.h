#ifndef NEMU_JOYPAD_H
#define NEMU_JOYPAD_H

#include <stdint.h>

typedef struct Joypad Joypad;
struct Joypad {
  struct {
    uint8_t pad1; /* $4016 */
    uint8_t pad2; /* $4017 */
  } reg;
  uint8_t times;
};

enum {
  BUTTON_A,
  BUTTON_B,
  BUTTON_SELECT,
  BUTTON_START,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
};

#endif
