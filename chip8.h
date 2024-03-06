#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define PIX_WIDTH (WINDOW_WIDTH / 64)
#define PIX_HEIGHT (WINDOW_HEIGHT / 32)

typedef struct {
	uint8_t V[16];
	uint16_t I;
	uint16_t SP;
	uint16_t PC;
	uint8_t delay;
	uint8_t sound;
	uint8_t memory[4096];
	uint16_t stack[16];
} Chip8;

typedef struct {
	SDL_Rect rect;
	bool active;
} pixel_t;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	pixel_t display[32][64];
	bool keys[16];
} sdl_t;

int init_sdl(sdl_t *sdl);
void init_cpu(Chip8 *cpu, const char *file_name);
void load_game_into_memory(Chip8 *cpu, const char *file_name);
void load_font_into_memory(Chip8 *cpu);
void init_pixels(sdl_t *sdl);
void draw_rectangles(sdl_t *sdl);
void set_pixel(sdl_t *sdl, int x, int y, bool value);
void draw_graphics(sdl_t *sdl);
void final_cleanup(sdl_t sdl);
void print_memory(Chip8 *cpu);
uint16_t fetch(Chip8 *cpu);

#endif
