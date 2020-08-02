#include <stdio.h>
#include <string.h>
#include "audio/audio.h"

void audio_init(int argc, char **argv, Audio *audio) {
  al_install_audio();
  al_reserve_samples(1);
  audio->buf = malloc(sizeof(float) * 4096);
  audio->nbuf = 0;
}

void audio_update(Audio *audio) {
  ALLEGRO_SAMPLE *s = al_create_sample(audio->buf,
      audio->nbuf,
      44100,
      ALLEGRO_AUDIO_DEPTH_FLOAT32,
      ALLEGRO_CHANNEL_CONF_1,
      false);
  al_play_sample(s, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  memset(audio->buf, 0, sizeof(float) * 4096);
  audio->nbuf = 0;
}
