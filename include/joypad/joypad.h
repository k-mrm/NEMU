#ifndef NEMU_JOYPAD_H
#define NEMU_JOYPAD_H

#include <stdint.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

enum button {
  BUTTON_A,
  BUTTON_B,
  BUTTON_SELECT,
  BUTTON_START,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
};

typedef struct Joypad Joypad;
struct Joypad {
  struct {
    uint8_t pad1; /* $4016 */
    uint8_t pad2; /* $4017 */
  } reg;
  struct {
    enum button pad1;
    enum button pad2;
  } btnstate;
};

void joypad_init(Joypad *);
void joypad_write(Joypad *, int, uint8_t);
uint8_t joypad_read(Joypad *, int);

#endif
