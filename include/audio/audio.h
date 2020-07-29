#ifndef NEMU_AUDIO_H
#define NEMU_AUDIO_H

#include <allegro5/allegro_audio.h>

typedef struct Audio Audio;
struct Audio {
  float *buf;
  size_t nbuf;
};

void audio_init(int, char **, Audio *);

#endif
