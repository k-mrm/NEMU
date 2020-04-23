#ifndef NEMU_LOG_H
#define NEMU_LOG_H

#define log_error(...) fprintf(stderr, __VA_ARGS__)

void nemu_error(char *, ...);
void panic(char *, ...);


#endif
