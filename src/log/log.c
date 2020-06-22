#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "log/log.h"

void panic(char *msg, ...) {
  va_list args;
  va_start(args, msg);
  log_error("[panic] ");
  vfprintf(stderr, msg, args);
  log_error("\n");
  va_end(args);

  exit(1);
}
