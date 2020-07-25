#include "apu/envelope.h"

void envelope_clock(struct envelope *e) {
  if(e->start) {
    e->start = 0;
    e->decay = 15;
    e->divider = e->volume;
  }
  else {
    if(e->divider == 0) {
      e->divider = e->volume;

      if(e->decay == 0) {
        if(e->loop)
          e->decay = 15;
      }
      else {
        e->decay--;
      }
    }
    else {
      e->divider--;
    }
  }
}

