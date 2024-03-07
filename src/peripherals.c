#include "chip8.h"
#include "peripherals.h"

#include <SDL2/SDL.h>

void handle_key_press(sdl_t *sdl, SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_1: sdl->keys[0x0] = true; break;
			case SDLK_2: sdl->keys[0x1] = true; break;
			case SDLK_3: sdl->keys[0x2] = true; break;
			case SDLK_4: sdl->keys[0x3] = true; break;
			case SDLK_q: sdl->keys[0x4] = true; break;
			case SDLK_w: sdl->keys[0x5] = true; break;
			case SDLK_e: sdl->keys[0x6] = true; break;
			case SDLK_r: sdl->keys[0x7] = true; break;
			case SDLK_a: sdl->keys[0x8] = true; break;
			case SDLK_s: sdl->keys[0x9] = true; break;
			case SDLK_d: sdl->keys[0xA] = true; break;
			case SDLK_f: sdl->keys[0xB] = true; break;
			case SDLK_z: sdl->keys[0xC] = true; break;
			case SDLK_x: sdl->keys[0xD] = true; break;
			case SDLK_c: sdl->keys[0xE] = true; break;
			case SDLK_v: sdl->keys[0xF] = true; break;
		}
	}

	if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
			case SDLK_1: sdl->keys[0x0] = false; break;
			case SDLK_2: sdl->keys[0x1] = false; break;
			case SDLK_3: sdl->keys[0x2] = false; break;
			case SDLK_4: sdl->keys[0x3] = false; break;
			case SDLK_q: sdl->keys[0x4] = false; break;
			case SDLK_w: sdl->keys[0x5] = false; break;
			case SDLK_e: sdl->keys[0x6] = false; break;
			case SDLK_r: sdl->keys[0x7] = false; break;
			case SDLK_a: sdl->keys[0x8] = false; break;
			case SDLK_s: sdl->keys[0x9] = false; break;
			case SDLK_d: sdl->keys[0xA] = false; break;
			case SDLK_f: sdl->keys[0xB] = false; break;
			case SDLK_z: sdl->keys[0xC] = false; break;
			case SDLK_x: sdl->keys[0xD] = false; break;
			case SDLK_c: sdl->keys[0xE] = false; break;
			case SDLK_v: sdl->keys[0xF] = false; break;
		}
	}
}
