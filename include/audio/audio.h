#ifndef NEMU_AUDIO_H
#define NEMU_AUDIO_H

#include <AL/alut.h>
#include <AL/al.h>
#include <AL/alc.h>

typedef struct Audio Audio;
struct Audio {
  ALuint buffer[4];
  ALuint src;
};

void audio_init(int, char **, Audio *);

#endif
