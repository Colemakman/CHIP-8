#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "instructions.h"
#include "chip8.h"
#include "peripherals.h"

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

void init_cpu(Chip8 *cpu) {
	load_font_into_memory(cpu);
	// load_game_into_memory(cpu);
	
	cpu->SP = -1;
	cpu->PC = 0x200;
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
	bool active = true;
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			sdl->display[y][x].rect.x = x * PIX_WIDTH;
			sdl->display[y][x].rect.y = y * PIX_HEIGHT;
			sdl->display[y][x].rect.w = PIX_WIDTH;
			sdl->display[y][x].rect.h = PIX_HEIGHT;
			sdl->display[y][x].active = active;
			active = !active;
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
	for (int i = 0; i < 4096; i++) {
		printf("%04x\n", cpu->memory[i]);
	}
}

uint16_t fetch(Chip8 *cpu) {
	uint8_t left = cpu->memory[cpu->PC];
	uint8_t right = cpu->memory[cpu->PC + 1];
	
	uint16_t instr = (left << 8) | right;
	cpu->PC += 2;

	return instr;
}

int main(int argc, char *argv[]) {
	Chip8 cpu;
	sdl_t sdl;

	init_cpu(&cpu);
	init_sdl(&sdl);
	init_pixels(&sdl);

	SDL_Event event;
	int quit = 0;
	while (!quit) {

		uint16_t instr = fetch(&cpu);
		// decode
		// execute

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
	}
	
	final_cleanup(sdl);
}
