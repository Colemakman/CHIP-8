LDLIBS = -lm

all:
	gcc chip8.c instructions.c peripherals.c disassembler.c -o chip8 -Wall -Wextra -lSDL2 $(LDLIBS) -g -lncurses
dis:
	cc -o disassembler disassembler.c
