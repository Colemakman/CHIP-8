LDLIBS = -lm

all:
	gcc chip8.c instructions.c peripherals.c -o chip8 -Wall -Wextra -lSDL2 $(LDLIBS)
dis:
	cc -o disassembler disassembler.c
