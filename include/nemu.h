#ifndef NEMU_NEMU_H
#define NEMU_NEMU_H

#include "cassette/cassette.h"
#include "cpu/cpu.h"
#include "cpu/bus.h"
#include "cpu/interrupt.h"
#include "cpu/instruction.h"
#include "ram/ram.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "log/log.h"
#include "ppu/palette.h"
#include "ppu/ppu.h"
#include "gui/gui.h"
#include "joypad/joypad.h"
#include "audio/audio.h"

typedef struct NEMU NEMU;
struct NEMU {
  CPU cpu;
  CPUBus cpubus;
  RAM ram;
  PPU ppu;
  PPUBus ppubus;
  Joypad pad;
  Disp screen;
  Cassette *cassette;
  GUI gui;
  Audio audio;
};

void nemu_boot(int, char **, NEMU *, Cassette *);
void nemu_close(NEMU *);

#endif
