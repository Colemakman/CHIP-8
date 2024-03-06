#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <ncurses.h>
#include <SDL2/SDL.h>

#include "instructions.h"
#include "chip8.h"
#include "peripherals.h"
#include "disassembler.h"

int init_sdl(sdl_t *sdl) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Could not init SDL video: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	sdl->window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (sdl->window == NULL) {
		SDL_Log("Window creation failed: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
	if (sdl->renderer == NULL) {
		SDL_Log("Render creation failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(sdl->window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void init_cpu(Chip8 *cpu, const char *file_name) {
	load_font_into_memory(cpu);
	load_game_into_memory(cpu, file_name);
	
	cpu->SP = -1;
	cpu->PC = 0x200;
}

void load_game_into_memory(Chip8 *cpu, const char *file_name) {
	FILE *file = fopen(file_name, "rb");
	if (file == NULL) {
		printf("Opening file error\n");
		return;
	}

	uint16_t addr = 0x200;
	uint8_t byte;
	while (fread(&byte, sizeof(uint8_t), 1, file) == 1 && addr < 4096) { 
		cpu->memory[addr++] = (byte << 4) | (byte >> 4);
	}
	print_memory(cpu);

	fclose(file);
}


void load_font_into_memory(Chip8 *cpu) {
	uint8_t start_address = 0x050;
	uint8_t font[80] = { 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
					0x20, 0x60, 0x20, 0x20, 0x70, // 1
					0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
					0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
					0x90, 0x90, 0xF0, 0x10, 0x10, // 4
					0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
					0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
					0xF0, 0x10, 0x20, 0x40, 0x40, // 7
					0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
					0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
					0xF0, 0x90, 0xF0, 0x90, 0x90, // A
					0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
					0xF0, 0x80, 0x80, 0x80, 0xF0, // C
					0xE0, 0x90, 0x90, 0x90, 0xE0, // D
					0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
					0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	for (int i = 0; i < 80; i++) {
		cpu->memory[start_address + i] = font[i];
	}
}

void init_pixels(sdl_t *sdl) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			sdl->display[y][x].rect.x = x * PIX_WIDTH;
			sdl->display[y][x].rect.y = y * PIX_HEIGHT;
			sdl->display[y][x].rect.w = PIX_WIDTH;
			sdl->display[y][x].rect.h = PIX_HEIGHT;
			sdl->display[y][x].active = false;
	    }
	}
}

void draw_rectangles(sdl_t *sdl) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			if (sdl->display[y][x].active) {
				SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(sdl->renderer, &sdl->display[y][x].rect);
			} else {
				SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(sdl->renderer, &sdl->display[y][x].rect);
			}
		}
	}
}

void set_pixel(sdl_t *sdl, int x, int y, bool value) {
	sdl->display[y][x].active = value; 
}

void draw_graphics(sdl_t *sdl) {
	SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl->renderer);
	draw_rectangles(sdl);
	SDL_RenderPresent(sdl->renderer);
}


void final_cleanup(sdl_t sdl) {
	SDL_DestroyRenderer(sdl.renderer);
	SDL_DestroyWindow(sdl.window);
	SDL_Quit();
}

void print_memory(Chip8 *cpu) {
	bool skip = false;
	for (int i = 0x200; i < 0x400; i++) {
		printf("%02x", cpu->memory[i]);
		if (skip) {
			printf("\n"); 
		}
		skip = !skip;
	}
}

uint16_t fetch(Chip8 *cpu) {
	uint8_t left = cpu->memory[cpu->PC];
	uint8_t right = cpu->memory[cpu->PC + 1];
	
	uint16_t instr = (right << 4) | (left >> 4); 
	cpu->PC += 2;

	return instr;
}

void decode_execute(Chip8 *cpu, sdl_t *sdl, uint16_t instr) {
	uint8_t first_nib =  (instr >> 4) & 0x0F;
	uint8_t second_nib = (instr >> 2) & 0x03;
	uint8_t third_nib =  (instr >> 1) & 0x01;
	uint8_t fourth_nib = (instr >> 0) & 0x0F;

	uint16_t val234nib = (second_nib << 8) | (third_nib << 4) | fourth_nib;
	uint8_t val34nib = (third_nib << 4) | fourth_nib;

	if (instr == 0x0000) {
		return;
	}

	switch (first_nib) {
		case 0x0:
			switch (fourth_nib) {
				case 0x0: disp_clear(sdl); break;
				case 0xE: return_from_sub(cpu); break;
				default: printf("UNKNOWN COMMAND"); break;
			}
			break;
		case 0x1: jump_addr(cpu, val234nib); break;
		case 0x2: call(cpu, val234nib); break;
		case 0x3: skip_eq(cpu, second_nib, val34nib); break;
		case 0x4: skip_nq(cpu, second_nib, val34nib); break;
		case 0x5: skip_reg_eq(cpu, second_nib, third_nib); break;
		case 0x6: set_reg(cpu, second_nib, val34nib); break;
		case 0x7: add_val_to_reg(cpu, second_nib, val34nib); break;
		case 0x8: {
			switch (fourth_nib) {
				case 0x0: set_reg_to_reg(cpu, second_nib, third_nib); break;
				case 0x1: set_reg_to_reg_OR(cpu, second_nib, third_nib); break;
				case 0x2: set_reg_to_reg_AND(cpu, second_nib, third_nib); break;
				case 0x3: set_reg_to_reg_XOR(cpu, second_nib, third_nib); break;
				case 0x4: add_reg_to_reg(cpu, second_nib, third_nib); break;
				case 0x5: sub_reg_from_reg(cpu, second_nib, third_nib); break;
				case 0x6: r_shift_reg(cpu, second_nib); break;
				case 0x7: diff_reg(cpu, second_nib, third_nib); break;
				case 0xE: l_shift_reg(cpu, second_nib); break;
				default: printf("UNKNOWN COMMAND"); break;
			}
			break;
		}
		case 0x9: skip_reg_nq(cpu, second_nib, third_nib); break;
		case 0xA: set_i(cpu, val234nib); break;
		case 0xB: jump_pc(cpu, val234nib); break;
		case 0xC: set_reg_rand(cpu, second_nib, val34nib); break;
		case 0xD: draw_sprite(sdl, cpu, second_nib, third_nib, fourth_nib); break;
		case 0xE: {
			switch (fourth_nib) {
				case 0xE: skip_if_pressed(sdl, cpu, second_nib); break;
				case 0x1: skip_if_not_pressed(sdl, cpu, second_nib); break;
				default: printf("UNKNOWN COMMAND"); break;
			}
			break;
		}
		case 0xF: {
			switch (fourth_nib) {
				case 0x7: set_reg_to_delay(cpu, second_nib); break;
				case 0xA: store_key_in_reg(sdl, cpu); break;
				case 0x8: set_sound(cpu, second_nib); break;
				case 0xE: add_i(cpu, second_nib); break;
				case 0x9: set_i_to_sprite_location(cpu, second_nib); break;
				case 0x3: bcd(cpu, second_nib); break;
				case 0x5: {
					switch(third_nib) {
						case 0x1: set_delay(cpu, second_nib); break;
						case 0x5: reg_dump(cpu, second_nib); break;
						case 0x6: reg_load(cpu, second_nib); break;
						default: printf("UNKNOWN COMMAND"); break;
					}
					break;
				}
				default: printf("UNKNOWN COMMAND"); break;
			}
			break;
		default: printf("UNKNOWN COMMAND"); break;
		}
	}
}

int main(int argc, char *argv[]) {
	Chip8 cpu;
	sdl_t sdl;

	if (argc != 2) {
		printf("Usage: ./chip8 <FILENAME>");
		return EXIT_FAILURE;
	}

	const char* file_name = argv[1];

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	init_cpu(&cpu, file_name);
	init_sdl(&sdl);
	init_pixels(&sdl);

	SDL_Event event;
	int quit = 0;
	while (!quit) {

		uint16_t instr = fetch(&cpu);
		decode_execute(&cpu, &sdl, instr);

		draw_graphics(&sdl);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: quit = 1; break;
				default: printf("Unknown"); break;
			}
			handle_key_press(&sdl, event);
		}

		if (cpu.delay > 0) {
			cpu.delay--;
		}
		if (cpu.sound > 0) {
			cpu.sound--;
		}

		// 60hz = 16.6666... ms, but SDL_Delay takes uint values only
		SDL_Delay(17);
		//disassemble_chip8_op(instr);
		printw("%04x\n", instr);
		refresh();
		getch();

	}
	
	endwin();
	final_cleanup(sdl);
}
