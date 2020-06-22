#ifndef NEMU_LOG_H
#define NEMU_LOG_H

#define log_error(...) fprintf(stderr, __VA_ARGS__)

#ifndef NDEBUG
# define log_dbg(...) fprintf(stderr, __VA_ARGS__)
#else
# define log_dbg(...)
#endif

void panic(char *, ...);

#endif
