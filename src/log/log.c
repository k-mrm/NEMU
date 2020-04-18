#include <stdio.h>
#include <stdarg.h>

#include "log/log.h"

void nemu_error(char *msg, ...) {
    va_list args;
    va_start(args, msg);
    log_error("[error] ");
    vfprintf(stderr, msg, args);
    log_error("\n");
    va_end(args);
}
