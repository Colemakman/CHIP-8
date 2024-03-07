#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "instructions.h"
#include "chip8.h"
#include "disassembler.h"

void disp_clear(sdl_t *sdl) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			sdl->display[y][x].active = false;
		}
	}
}

void return_from_sub(Chip8 *cpu) {
	cpu->PC = cpu->stack[cpu->SP-1];
	cpu->stack[cpu->SP-1] = 0; 
	cpu->SP -= 1;
}

void jump_addr(Chip8 *cpu, uint16_t addr) {
	cpu->PC = addr;
}

void call(Chip8 *cpu, uint16_t addr) {
	cpu->SP += 1;	
	cpu->stack[cpu->SP-1] = cpu->PC;
	cpu->PC = addr;
}

void skip_eq(Chip8 *cpu, uint8_t reg, uint8_t val) {
	if (cpu->V[reg] == val) {
		cpu->PC += 2;
	}
}

void skip_nq(Chip8 *cpu, uint8_t reg, uint8_t val) {
	if (cpu->V[reg] != val) {
		cpu->PC += 2;
	}
}

void skip_reg_eq(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	if (cpu->V[regA] == cpu->V[regB]) {
		cpu->PC += 2;
	}
}

void set_reg(Chip8 *cpu, uint8_t reg, uint8_t val) {
	cpu->V[reg] = val;
}

void add_val_to_reg(Chip8 *cpu, uint8_t reg, uint8_t val) {
	cpu->V[reg] += val;
}

void set_reg_to_reg(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] = cpu->V[regB];
}

void set_reg_to_reg_OR(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] |= cpu->V[regB];
}

void set_reg_to_reg_AND(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] &= cpu->V[regB];
}

void set_reg_to_reg_XOR(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] ^= cpu->V[regB];
}

void add_reg_to_reg(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] += cpu->V[regB];
}

void sub_reg_from_reg(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] -= cpu->V[regB];
}

void r_shift_reg(Chip8 *cpu, uint8_t reg) {
	cpu->V[0xE] = cpu->V[reg] & 0x1;
	cpu->V[reg] >>= 1;
}

void diff_reg(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	cpu->V[regA] = cpu->V[regB] - cpu->V[regA];
}

void l_shift_reg(Chip8 *cpu, uint8_t reg) {
	cpu->V[0xE] = cpu->V[reg] & 0x80;
	cpu->V[reg] <<= 1;
}

void skip_reg_nq(Chip8 *cpu, uint8_t regA, uint8_t regB) {
	if (cpu->V[regA] != cpu->V[regB]) {
		cpu->PC += 2;
	}
}

void set_i(Chip8 *cpu, uint16_t val) {
	cpu->I = val;
}

void jump_pc(Chip8 *cpu, uint16_t val) {
	cpu->PC = cpu->V[0] + val;
}

void set_reg_rand(Chip8 *cpu, uint8_t reg, uint8_t val) {
	cpu->V[reg] = (uint8_t)((rand() & 256) & val);
}

void draw_sprite(sdl_t *sdl, Chip8 *cpu, uint8_t regA, uint8_t regB, uint8_t val) {

	// modulo to fix wrapping problems
	uint8_t y_loc = cpu->V[regB] % 32;
	
	// set VF to 0
	cpu->V[0xE] = 0;

	uint8_t sprite_data[5];
	for (int i = 0; i < val; i++) {

		// reset x to left
		uint8_t x_loc = cpu->V[regA] % 64;

		// get the sprite row
		sprite_data[i] = cpu->memory[cpu->I + i];
		for (int j = 0; j < 8; j++) {
			// extract bits from MSB -> LSB using cursed logic
			bool bit = (sprite_data[i] >> (7-j)) & 1;

			// if bit is on, and pixel is also on set VF to 1
			if (bit == true && sdl->display[y_loc][x_loc].active == true) {
				cpu->V[0xE] = 1;

			// if bit is on bit pixel is not, turn pixel on
			} else if (bit == true && sdl->display[y_loc][x_loc].active == false) {
				sdl->display[y_loc][x_loc].active = true;
			}

			// increment x
			x_loc++;		
		}
		
		// if we try and draw below the screen, exit out early
		y_loc++;
		if (y_loc >= 31) {
			return;
		}
	}
}

void skip_if_pressed(sdl_t *sdl, Chip8 *cpu, uint8_t reg) {
	if (sdl->keys[cpu->V[reg]] == true) {
		cpu->PC += 2;
	}
}

void skip_if_not_pressed(sdl_t *sdl, Chip8 *cpu, uint8_t reg) {
	if (sdl->keys[cpu->V[reg]] == false) {
		cpu->PC += 2;
	}
}

void set_reg_to_delay(Chip8 *cpu, uint8_t reg) {
	cpu->V[reg] = cpu->delay;
}

// this may cause problems.
void store_key_in_reg(sdl_t *sdl, Chip8 *cpu) {
	for (int key_index = 0; key_index < 16; key_index++) {
		if (sdl->keys[key_index] == true) {
			cpu->V[0xF] = 0xF & key_index;
			return;
		} 
	}
	cpu->PC -= 2;
}

void set_delay(Chip8 *cpu, uint8_t reg) {
	cpu->delay = cpu->V[reg];
}

void set_sound(Chip8 *cpu, uint8_t reg) {
	cpu->sound = cpu->V[reg];
}

void add_i(Chip8 *cpu, uint8_t reg) {
	cpu->I += cpu->V[reg];
}

void set_i_to_sprite_location(Chip8 *cpu, uint8_t reg) {
	uint8_t reg_val = cpu->V[reg];

	cpu->I = 0x050 + reg_val * 5;
}

void bcd(Chip8 *cpu, uint8_t reg) {
	uint16_t i = cpu->I;
	uint8_t val = cpu->V[reg];

	uint8_t right = val % 10;
	uint8_t mid = (val / 10) % 10;
 	uint8_t left = val / 100;
	
	cpu->memory[i] = left;
	cpu->memory[i+1] = mid;
	cpu->memory[i+2] = right;
}

void reg_dump(Chip8 *cpu, uint8_t reg) {
	uint16_t i = cpu->I;
	
	for (int x = 0; x < reg; x++) {
		cpu->memory[i + x] = cpu->V[x];
	}
}

void reg_load(Chip8 *cpu, uint8_t reg) {
	uint16_t i = cpu->I;

	for (int x = 0; x < reg; x++) {
		cpu->V[x] = cpu->memory[i + x];
	}
}
