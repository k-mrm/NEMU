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

typedef struct NEMU NEMU;
struct NEMU {
  CPU cpu;
  CPUBus cpubus;
  RAM ram;
  PPU ppu;
  PPUBus ppubus;
  APU apu;
  Joypad pad;
  Disp screen;
  GUI gui;
  Cassette *cassette;
};

int nemu_start(NEMU *, int *, char **);

#endif
