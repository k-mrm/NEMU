#include <stdio.h>
#include "audio/audio.h"

void audio_init(int argc, char **argv, Audio *audio) {
  alutInit(&argc, argv);
  ALCdevice *dev = alcOpenDevice(NULL);
  ALCcontext *ctx = alcCreateContext(dev, NULL);
  alGenBuffers(4, audio->buffer);
  alGenSources(1, &audio->src);
}
