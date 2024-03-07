LDLIBS = -lm

all:
	gcc src/chip8.c src/instructions.c src/peripherals.c src/disassembler.c -o chip8 -Wall -Wextra -lSDL2 $(LDLIBS) -g -lncurses
