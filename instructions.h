#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "chip8.h"

void disp_clear(sdl_t *sdl); // 00E0
void return_from_sub(Chip *cpu); // 00EE
void goto(Chip8 *cpu, uint16_t addr); // 1NNN
void call(Chip8 *cpu, uint16_t addr); // 2NNN
void skip_eq(Chip8 *cpu, uint8_t reg, uint8_t val); // 3XNN
void skip_nq(Chip8 *cpu, uint8_t reg, uint8_t val); // 4XNN
void skip_reg_eq(Chip8 *cpu, uint8_t regA, uint8_t regB); // 5XY0
void set_reg(Chip8 *cpu, uint8_t reg, uint8_t val); // 6XNN
void add_val_to_reg(Chip8 *cpu, uint8_t reg, uint8_t val); // 7XNN
void set_reg_to_reg(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY0
void set_reg_to_reg_OR(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY1
void set_reg_to_reg_AND(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY2
void set_reg_to_reg_XOR(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY3
void add_reg_to_reg(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY4
void sub_reg_from_reg(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY5
void r_shift_reg(Chip8 *cpu, uint8_t reg); // 8XY6
void diff_reg(Chip8 *cpu, uint8_t regA, uint8_t regB); // 8XY7
void l_shift_reg(Chip8 *cpu, uint8_t reg); // 8XYE
void skip_reg_nq(Chip8 *cpu, uint8_t regA, uint8_t regB); // 9XY0
void set_i(Chip8 *cpu, uint16_t val); // ANNN
void jump_pc(Chip8 *cpu, uint16_t val); // BNNN
void set_reg_rand(Chip8 *cpu, uint8_t reg, uint8_t val); // CXNN
void draw_sprite(sdl_t *sdl, Chip8 *cpu, uint8_t regA, uint8_t regB, uint8_t val); // DXYN
void skip_if_pressed(sdl_t *sdl, chip8 *cpu, uint8_t reg); // EX9E
void skip_if_not_pressed(sdl_t *sdl, chip8 *cpu, uint8_t reg); // EXA1
void set_reg_to_delay(Chip8 *cpu, uint8_t reg); // FX07
void store_key_in_reg(sdl_t *sdl, Chip8 *cpu, uint8_t reg, SDL_Event key); // FX0A
void set_delay(Chip8 *cpu, uint8_t reg); // FX15
void set_sound(Chip8 *cpu, uint8_t reg); // FX18
void add_i(Chip8 *cpu, uint8_t reg); // FX1E
void set_i_to_sprite_location(chip8 *cpu, uint8_t reg); // FX29
void bcd(); // FX33
void reg_dump(Chip8 *cpu, uint8_t reg); // FX55
void reg_load(Chip8 *cpu, uint8_t reg); // FX65

#endif
