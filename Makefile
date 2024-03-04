all:
	gcc -o chip8 chip8.c -Wall -Wextra -lSDL2
dis:
	cc -o disassembler disassembler.c
