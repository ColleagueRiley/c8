
#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER

#include <stdio.h>
#include "RGFW.h"

#include "constant.h"
#include "draw.h"

union opcode opcode;
uint8_t memory[4096];
uint8_t registers[16];
uint16_t i;
uint16_t pc;

uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
bool keys[16];

uint16_t stack[16];
uint16_t sp;

uint8_t sound_timer;
uint8_t delay_timer;

int main(int argc, char* args[]) {
    if (argc != 2) {
        printf("usage: c8 game\n");
        printf("    game: Chip8 binary file to play\n");
        return -1;
    }

    /* load rom file */
    char* filename = args[1];

    FILE* game = fopen(filename, "rb");
    
    if (!game) {
      fprintf(stderr, "Unable to open file '%s'!\n", filename);
      return -1;
    }

    fread(&memory[PC_START], 1, 4096 - PC_START, game);
    fclose(game);
    /*  */

    /* init emulator */
    memcpy(memory, font, sizeof(font));
    memset(pixels, OFF_COLOR, sizeof(pixels));
    memset(stack, 0, sizeof(stack));
    memset(registers, 0, sizeof(registers));
    memset(keys, 0, sizeof(keys));

    pc = PC_START;
    opcode.instruction = 0;
    i = 0;
    sp = 0;
    sound_timer = 0;
    delay_timer = 0;

    srand(time(NULL)); /* generate random seed */

    RGFW_window* window = RGFW_createWindowPointer("c8", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGFW_NO_RESIZE);

    int* screenSize = RGFW_getScreenSize(window);
    window->x = (screenSize[0] + window->w) / 4;
    window->y = (screenSize[1] + window->h) / 4;
    
    uint32_t start_tick;
    uint32_t frame_speed;

    bool running = true;

    while (running) {
        opcode.instruction = memory[pc] << 8 | memory[pc + 1];

        pc += 2;
        uint8_t vx = registers[opcode.x];
        uint8_t vy = registers[opcode.y];

        switch (opcode.op) {
            case 0x0: {
                switch (opcode.n) {
                case 0x0: 
                    memset(pixels, OFF_COLOR, sizeof(pixels));
                    break;
                case 0xE: 
                    pc = stack[--sp]; 
                    break;
                default:    
                    fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n", opcode.instruction, pc); 
                    break;
                }

                break;
            }
            
            case 0x1: 
                pc = opcode.addr; 
                break;

            case 0x2: 
                stack[sp++] = pc;
                pc = opcode.addr; 
                break;

            case 0x3: 
                pc += (vx == opcode.kk) ? 2 : 0;
                break;

            case 0x4: 
                pc += (vx != opcode.kk) ? 2 : 0;
                break;

            case 0x5: 
                pc += (vx == vy) ? 2 : 0;
                break;

            case 0x6: 
                registers[opcode.x] = opcode.kk; 
                break;

            case 0x7: 
                registers[opcode.x] = vx + opcode.kk; 
                break;

            case 0x8: {
                switch (opcode.n) {
                    case 0x0: 
                        registers[opcode.x] = vy; 
                        break;

                    case 0x1: 
                        registers[opcode.x] = vx | vy; 
                        break;

                    case 0x2: 
                        registers[opcode.x] = vx & vy; 
                        break;

                    case 0x3: 
                        registers[opcode.x] = vx ^ vy; 
                        break;

                    case 0x4: 
                        registers[CARRY_REGISTER] = vx + vx > 255;
                        registers[opcode.x] = vy + vx;
                        break;

                    case 0x5: 
                        registers[CARRY_REGISTER] = vx > vy;
                        registers[opcode.x] = vx - vy;
                        break;

                    case 0x6: 
                        registers[CARRY_REGISTER] = registers[opcode.x] & 1;
                        registers[opcode.x] >>= 2;
                        break;

                    case 0x7: 
                        registers[CARRY_REGISTER] = vy > vx;
                        registers[opcode.x] = vy - vx;
                        break;

                    case 0xE: 
                        registers[CARRY_REGISTER] = (bool)(registers[opcode.x] & 0x80);
                        registers[opcode.x] <<= 2;
                        break;
                    default: 
                        fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n", opcode.instruction, pc);
                        break;
                }
                
                break;
            }

            case 0x9: 
                pc += (vx != vy) ? 2 : 0;
                break;

            case 0xA: 
                i = opcode.addr; 
                break;

            case 0xB: 
                pc = opcode.addr + registers[0]; 
                break;

            case 0xC: 
                registers[opcode.x] = (rand() % 256) & opcode.kk;
                break;

            case 0xD: {
                registers[CARRY_REGISTER] = 0;
                
                int x, y;
                for (y = 0; y < opcode.n; y++) {
                    for (x = 0; x < 8; x++) {
                        uint8_t pixel = memory[i + y];
                        if (pixel & (0x80 >> x)) {
                            int index =
                                (registers[opcode.x] + x) % SCREEN_WIDTH +
                                ((registers[opcode.y] + y) % SCREEN_HEIGHT) * SCREEN_WIDTH;
                            if (pixels[index] == ON_COLOR) {
                                registers[CARRY_REGISTER] = 1;
                                pixels[index] = OFF_COLOR;
                            } else {
                                pixels[index] = ON_COLOR;
                            }

                            drawMono(window, pixels);
                            RGFW_swapBuffers(window);
                        }
                    }
                }

                break;
            }

            case 0xE:
                switch (opcode.kk) {
                    case 0x9E: 
                        pc += (RGFW_isPressedS(window, key_map[vx])) ? 2 : 0;
                        break;

                    case 0xA1: 
                        pc += (!RGFW_isPressedS(window, key_map[vx])) ? 2 : 0;
                        break;
                    default:
                        fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n", opcode.instruction, pc);
                        break;
                }

                break;

            case 0xF:
                switch (opcode.kk) {
                    case 0x07: 
                        registers[opcode.x] = delay_timer; 
                        break;
                    
                    case 0x0A: {
                        pc -= 2;
                        
                        int i;
                        for (i = 0; i < 16; i++) {
                            if (RGFW_isPressedS(window, key_map[i])) {
                                registers[opcode.x] = i;
                                pc += 2;
                                break;
                            }
                        }
                        break;
                    }

                    case 0x15: 
                        delay_timer =  vx; 
                        break;

                    case 0x18: 
                        sound_timer = vx;
                        break;

                    case 0x1E: 
                        i = i + vx; 
                        break;

                    case 0x29: 
                        i = vx * 5; 
                        break;

                    case 0x33: 
                        memory[i]     = registers[opcode.x] / 100;
                        memory[i + 1] = (registers[opcode.x] / 10) % 10;
                        memory[i + 2] = registers[opcode.x] % 10;
                        break;

                    case 0x55: 
                        memcpy(memory + i, registers, opcode.x + 1);
                        break;

                    case 0x65: 
                        memcpy(registers, memory + i, opcode.x + 1);
                        break;
                    default:   
                        fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n", opcode.instruction, pc); break;
                        break;
                }
                
                break;
            default: 
                fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n", opcode.instruction, pc);
                break;
        }

        if (delay_timer > 0) 
            delay_timer--;
        if (sound_timer > 0) 
            sound_timer--;

        RGFW_checkEvents(window);
        if (window->event.type == RGFW_quit)
            running = false;
    }
        
    RGFW_closeWindow(window);
    return 0;
}
