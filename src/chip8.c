#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE 4096
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct {
    int memory[RAM_SIZE];
} RAM;

typedef struct {
    bool pixels[DISPLAY_WIDTH][DISPLAY_HEIGHT];
} Display;

typedef struct {
    int8_t value;
} Byte;

typedef struct {
    int16_t value;
} Word;

typedef struct {
    RAM ram;
    Display display;
    Byte PC;
    Word I;
    Word stack;
    Byte delay;
    Byte sound;
} CHIP8;

void initialise_memory(CHIP8 *chip) {
    for (int i = 0; i < RAM_SIZE; i++) {
        chip->ram.memory[i] = 0;
    }
}

void initialise_display(CHIP8 *chip) {
    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        for (int j = 0; j < DISPLAY_HEIGHT; j++) {
            chip->display.pixels[i][j] = false;
        }
    }
}

void load_font_into_memory(CHIP8 *chip) {
    const int FONT[16][5] = {
        {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
        {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
        {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
        {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
        {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
        {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
        {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
        {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
        {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
        {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
        {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
        {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
        {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
        {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
        {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
        {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
    };

    const int FONT_START_ADDRESS = 0x050;
    int offset = 0;
    
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 5; j++) {
            chip->ram.memory[FONT_START_ADDRESS + offset] = FONT[i][j];
            offset++;
        }
    }
}

void initialise_chip(CHIP8 *chip) {
    initialise_display(chip);
    initialise_memory(chip);
    load_font_into_memory(chip);
}

void print_memory(CHIP8 *chip) {
    for (int i = 0; i < RAM_SIZE; i++) {
        printf("%x", chip->ram.memory[i]);
        if (i % 50 == 0) { printf("\n"); }
    }
}

unsigned short join_bytes(unsigned char high_byte, unsigned char low_byte) {
    unsigned short result = (high_byte << 8) | low_byte;
    return result;
}

int fetch(CHIP8 *chip) {
    Byte address = chip->PC;
    int low = chip->ram.memory[address.value];
    int high = chip->ram.memory[address.value + 1];

    chip->PC.value += 2;

    return join_bytes(high, low);
}

int main() {
    CHIP8 chip;
    initialise_chip(&chip);
    print_memory(&chip);

    return 0;
}
