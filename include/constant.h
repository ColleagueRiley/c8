#include <stdbool.h>
#include <stdint.h>

enum {
    SCREEN_WIDTH = 64,
    SCREEN_HEIGHT = 32,
    PC_START = 0x200,
    CARRY_REGISTER = 0xF,
    ON_COLOR = 0xFFFFFFFF,
    OFF_COLOR = 0xFF000000,
    FRAMES_PER_SECOND = 60,
    MILLISECONDS_PER_CYCLE = 1000 / FRAMES_PER_SECOND,
    SCALE = 10
};


char* key_map[16] = { "1", "2", "3", "4", "Q", "W", "E", "R", "A", "S", "D", "F", "Z", "X", "C", "V" };

static const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
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

union opcode {
    uint16_t instruction;
    struct {
      uint8_t n : 4;
      uint8_t y : 4;
      uint8_t x : 4;
      uint8_t op : 4;
    };
    struct {
      uint8_t kk : 8;
    };
    struct {
      uint16_t addr : 12;
    };
};