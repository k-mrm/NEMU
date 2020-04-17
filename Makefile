CC := gcc
CFLAGS = -Wall -Wextra -I ./include/ -g
SRCROOT = ./src
SRCDIRS := $(shell find $(SRCROOT) -type d)
SRCS=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJS=$(SRCS:.c=.o)
PROGRAM = emu
.SUFFIXES: .c .o
.PHONY: clean

emu: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS) $(PROGRAM)
