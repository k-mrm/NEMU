CC := gcc
CFLAGS = -Wall -Wextra -I ./include/ -O3 -DNDEBUG
LDFLAGS := -lallegro -lallegro_main -lallegro_primitives -lallegro_audio
SRCROOT = ./src
SRCDIRS := $(shell find $(SRCROOT) -type d)
SRCS=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJS=$(SRCS:.c=.o)
PROGRAM = emu
.SUFFIXES: .c .o
.PHONY: clean

emu: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROGRAM)
